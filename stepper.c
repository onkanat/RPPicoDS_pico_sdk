/**
 * @file stepper.c
 * @brief Step motor kontrol fonksiyonları
 * @see \ref howto_stepper
 */

#include "pico_training_board.h"

/**
 * @brief Step motoru yarım adım sekans deseni
 * 
 * Bu dizi, 4 fazlı bir step motorunu yarım adım modunda kontrol etmek için 8 adımlı bir sekans tanımlar.
 * Her satır, sekanstaki bir adımı ve her sütun bir motor bobininin durumunu temsil eder (1 = enerjili, 0 = enerjisiz).
 * 
 * Sekans, tek bobinleri ve bitişik bobin çiftlerini sırayla enerjilendirerek daha düzgün motor dönüşü sağlar:
 * - Sekans başına 8 adım (satırlar)
 * - 4 bobin/faz (sütunlar)
 * - Saat yönünde dönüş için standart yarım adım deseni izler
 * 
 * @note Sürekli dönüş sağlamak için sekans son adımdan ilk adıma döner
 */
static const uint8_t step_sequence[8][4] = {
    {1, 0, 0, 0},  /** Adım 1: İlk bobin sadece */
    {1, 1, 0, 0},  /** Adım 2: İlk ve ikinci bobinler */
    {0, 1, 0, 0},  /** Adım 3: İkinci bobin sadece */
    {0, 1, 1, 0},  /** Adım 4: İkinci ve üçüncü bobinler */
    {0, 0, 1, 0},  /** Adım 5: Üçüncü bobin sadece */
    {0, 0, 1, 1},  /** Adım 6: Üçüncü ve dördüncü bobinler */
    {0, 0, 0, 1},  /** Adım 7: Dördüncü bobin sadece */
    {1, 0, 0, 1}   /** Adım 8: Dördüncü ve ilk bobinler */
};

static const uint motor_pins[4] = {
    STEP_MOTOR_A1,
    STEP_MOTOR_A2,
    STEP_MOTOR_B1,
    STEP_MOTOR_B2
};

// Global değişkenler
static volatile motor_state_t motor_state = MOTOR_STOPPED;
static volatile bool emergency_stop = false;

/**
 * @brief Step motor GPIO pinlerini başlatır
 * 
 * Bu fonksiyon, 4 fazlı bir step motorunu kontrol etmek için kullanılan GPIO pinlerini yapılandırır.
 * Her pini başlatır ve motor bobinlerini sürmek için çıkış olarak ayarlar.
 * 
 * @note motor_pins[] dizisinin step motorunun 4 fazı için geçerli GPIO pin numaralarını içerdiği varsayılır
 */
void init_step_motor(void) {
    // Motor kontrol pinlerini başlat
    for (int i = 0; i < 4; i++) {
        gpio_init(motor_pins[i]);
        gpio_set_dir(motor_pins[i], GPIO_OUT);
    }
}

// Motor durumunu kontrol et
/**
 * @brief Motorun çalışıp çalışmadığını kontrol eder
 * @return true motor çalışıyorsa, aksi halde false
 */
bool is_motor_running(void) {
    return (motor_state == MOTOR_RUNNING);
}

// Motor durumunu döndür
/**
 * @brief Motorun mevcut durumunu döndürür
 * @return motor_state_t MOTOR_STOPPED, MOTOR_RUNNING veya MOTOR_ERROR
 */
motor_state_t get_motor_state(void) {
    return motor_state;
}

// Motoru durdur
/**
 * @brief Step motor için acil durdurma fonksiyonu
 * 
 * Bu fonksiyon, step motorunu acil olarak durdurur:
 * - Acil durdurma bayrağını ayarlar
 * - Motor durumunu durdurulmuş olarak günceller
 * - Tüm motor bobinlerini enerjisiz hale getirir (GPIO pinlerini düşük seviyeye ayarlar)
 * 
 * @note motor_pins[] dizisinin step motorunun 4 fazı için geçerli GPIO pin numaralarını içerdiği varsayılır
 * 
 * @warning Bu fonksiyon, motoru yavaşlatmadan hemen durdurur, bu da mekanik strese neden olabilir
 */
void step_stop(void) {
    emergency_stop = true;
    motor_state = MOTOR_STOPPED;
    
    // Tüm motor pinlerini enerjisiz hale getir
    for (int i = 0; i < 4; i++) {
        gpio_put(motor_pins[i], 0);
    }
}

/**
 * @brief Belirtilen yön, hız ve devir sayısı ile step motor dönüşünü kontrol eder
 *
 * @param direction Motor dönüş yönü (CW veya CCW)
 * @param speed Motor hızı (adım/saniye)
 * @param revolutions Yapılacak tam devir sayısı
 * 
 * @details Bu fonksiyon, yarım adım modu (her devir için 8 adım) kullanarak bir step motoru kontrol eder.
 * Motor durumu yönetimi, acil durdurma işlevselliği ve hassas zamanlama kontrolü sağlar.
 * Motor zaten çalışıyorsa fonksiyon başlamaz.
 * 
 * @note Fonksiyon, motor_pins[] dizisinde tanımlanan GPIO pinlerini ve step_sequence[][] dizisinde tanımlanan adım sekanslarını kullanır.
 * 
 * Kullanılan global değişkenler:
 * - motor_state: Motorun mevcut durumu
 * - emergency_stop: Acil durdurma bayrağı
 * - motor_pins[]: Motor kontrolü için GPIO pin numaraları dizisi
 * - step_sequence[][]: Adım sekans desenlerini içeren dizi
 */
void step_turn(motor_direction_t direction, uint speed, float revolutions) {
    if (motor_state == MOTOR_RUNNING) {
        printf("Motor zaten çalışıyor.\n");
        return;
    }

    init_step_motor();
    // Motor durumunu güncelle
    motor_state = MOTOR_RUNNING;
    emergency_stop = false;

    // Toplam adım sayısını hesapla
    const int steps_per_revolution = 8; // Her bir tam tur için 8 adım (yarım adım modu)
    int total_steps = (int)(steps_per_revolution * revolutions);

    // Hız gecikmesi (mikrosaniye) hesapla
    uint delay_us = 1000000 / speed; // Hız (adım/saniye) ile gecikme hesaplama

    int step_index = 0; // Başlangıç adım indeksi
    // Döngü: Belirtilen adım sayısı boyunca motoru döndür
    for (int step = 0; step < total_steps; step++) {
        if (emergency_stop) {
            printf("Acil durdurma tetiklendi.\n");
            break;
        }

        // Adım sekansını motor pinlerine uygula
        for (int i = 0; i < 4; i++) {
            gpio_put(motor_pins[i], step_sequence[step_index][i]);
        }

        // Yön belirle ve adım dizinini güncelle
        if (direction == CW) {
            step_index = (step_index + 1) % steps_per_revolution;
        } else {
            step_index = (step_index - 1 + steps_per_revolution) % steps_per_revolution;
        }

        // Belirtilen hızda gecikme
        sleep_us(delay_us);
    }

    // Motoru durdur
    step_stop();
    printf("Motor hareketi tamamlandı.\n");
}
