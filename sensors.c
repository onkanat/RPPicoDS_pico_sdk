/**
 * @file sensors.c
 * @brief Pico Eğitim Kartı için PIR ve ultrasonik sensör fonksiyonlarının uygulanması
 * @see \ref howto_sensors
 */

#include "pico_training_board.h"


/**
 * @brief PIR hareket sensörünü başlatır
 * 
 * PIR sensör pinini giriş olarak yapılandırır
 */
void init_pir(void) {
    gpio_init(PIR_DETECTOR);
    gpio_set_dir(PIR_DETECTOR, GPIO_IN);
    gpio_pull_down(PIR_DETECTOR);  // Kararlı okuma için pull-down kullan
}

/**
 * @brief PIR sensörü tarafından hareket algılanıp algılanmadığını kontrol eder
 * 
 * @return Hareket algılandıysa true, aksi halde false
 */
bool detect_motion(void) {
    return gpio_get(PIR_DETECTOR);
}

// Ultrasonik sensör hesaplamaları için sabitler
#define SOUND_SPEED      343.0f     // Oda sıcaklığında ses hızı (m/s)
#define SENSOR_TIMEOUT   26100      // Zaman aşımı süresi (mikrosaniye) (yaklaşık 4.5m menzil)
#define TRIGGER_PULSE    10         // Tetikleme darbe genişliği (mikrosaniye)

/**
 * @brief Ultrasonik sensör pinlerini başlatır
 * 
 * Tetikleme pinini çıkış ve yankı pinini giriş olarak yapılandırır
 */
void init_ultrasonic(void) {
    // Tetikleme pinini çıkış olarak yapılandır
    gpio_init(ULTRA_SONIC_TR);
    gpio_set_dir(ULTRA_SONIC_TR, GPIO_OUT);
    gpio_put(ULTRA_SONIC_TR, 0);    // Tetiklemeyi düşük başlat

    // Yankı pinini giriş olarak yapılandır
    gpio_init(ULTRA_SONIC_EC);
    gpio_set_dir(ULTRA_SONIC_EC, GPIO_IN);
}

/**
 * @brief Ultrasonik sensör kullanarak mesafe ölçer
 * 
 * @return float Santimetre cinsinden mesafe veya ölçüm başarısız olursa -1.0
 *
 * @note Ölçüm aralığı tipik olarak ~2 cm ile ~450 cm arasındadır. Zaman aşımlarında -1.0 döner.
 * @note Ölçüm öncesi `init_ultrasonic()` çağrılmış olmalıdır.
 */
float measure_distance(void) {
    uint64_t start_time, end_time;
    float duration;

    // Tetikleme darbesi gönder
    gpio_put(ULTRA_SONIC_TR, 1);
    sleep_us(TRIGGER_PULSE);
    gpio_put(ULTRA_SONIC_TR, 0);

    // Yankı pininin yüksek olmasını bekle
    uint32_t timeout_start = time_us_32();
    while (!gpio_get(ULTRA_SONIC_EC)) {
        if (time_us_32() - timeout_start > SENSOR_TIMEOUT) {
            return -1.0f;  // Zaman aşımı oluştu
        }
    }

    // Başlangıç zamanını al
    start_time = time_us_64();

    // Yankı pininin düşük olmasını bekle
    while (gpio_get(ULTRA_SONIC_EC)) {
        if (time_us_32() - timeout_start > SENSOR_TIMEOUT) {
            return -1.0f;  // Zaman aşımı oluştu
        }
    }

    // Bitiş zamanını al
    end_time = time_us_64();

    // Süreyi ve mesafeyi hesapla
    duration = (float)(end_time - start_time);
    float distance = (SOUND_SPEED * duration * 0.0001f) / 2.0f;  // Santimetre cinsinden mesafe

    return distance;
}


/**
 * @brief Zaman aşımı ile hareket algılamayı bekler
 * 
 * @param timeout_ms Beklenecek maksimum süre (milisaniye), 0 ise zaman aşımı yok
 * @return Hareket algılandıysa true, zaman aşımı oluştuysa false
 *
 * @code{.c}
 * if (wait_for_motion(5000)) {
 *   // 5 saniye içinde hareket algılandı
 * } else {
 *   // zaman aşımı
 * }
 * @endcode
 */
bool wait_for_motion(uint32_t timeout_ms) {
    absolute_time_t start_time = get_absolute_time();
    
    while (!detect_motion()) {
        if (timeout_ms > 0 && 
            absolute_time_diff_us(start_time, get_absolute_time()) > timeout_ms * 1000) {
            return false;  // Zaman aşımı oluştu
        }
        tight_loop_contents();  // CPU kullanımını optimize et
    }
    
    return true;  // Hareket algılandı
}

/**
 * @brief Birden fazla ölçümden ortalama mesafeyi alır
 * 
 * @param num_samples Ortalama alınacak örnek sayısı
 * @param delay_ms Örnekler arasındaki gecikme süresi (milisaniye)
 * @return float Santimetre cinsinden ortalama mesafe veya ölçümler başarısız olursa -1.0
 *
 * @note Negatif dönen ölçümler (zaman aşımı) ortalamaya dahil edilmez.
 */
float get_average_distance(uint8_t num_samples, uint32_t delay_ms) {
    if (num_samples == 0) return -1.0f;

    float total = 0.0f;
    uint8_t valid_samples = 0;

    for (uint8_t i = 0; i < num_samples; i++) {
        float distance = measure_distance();
        if (distance > 0) {
            total += distance;
            valid_samples++;
        }
        if (delay_ms > 0) {
            sleep_ms(delay_ms);
        }
    }

    return (valid_samples > 0) ? (total / valid_samples) : -1.0f;
}
