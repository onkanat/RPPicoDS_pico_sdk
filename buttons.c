/**
 * @file buttons.c
 * @brief Pico Eğitim Kartı için buton kontrol implementasyonu
 * @author Hakan KILIÇASLAN
 * @date 2024-12-4
 * 
 * Bu dosya, Pico Eğitim Kartı üzerindeki buton girişleri için fonksiyonlar sağlar,
 * debounce koruması, basma algılama ve zamanlama özelliklerini içerir.
 */
#include "pico_training_board.h"

/**
 * @struct button_state
 * @brief Bir butonun durumunu ve zamanlamasını takip eden yapı
 * 
 * @var last_press_time Son kayıtlı buton basma zamanı
 * @var is_pressed Butonun mevcut basılı durumu
 * @var was_pressed Butonun önceki basılı durumu
 * @var hold_duration Butonun basılı tutulma süresi (milisaniye cinsinden)
 */
static struct {
    absolute_time_t last_press_time;
    bool is_pressed;
    bool was_pressed;
    uint32_t hold_duration;
} button_states[3] = {0}; // YUKARI, TAMAM ve AŞAĞI butonları için dizi

/**
 * @brief Debounce korumalı temel buton basma kontrolü
 * 
 * @param gpio Kontrol edilecek GPIO pini
 * @return bool buton basılıysa (debounce sonrası) true, değilse false
 * 
 * @details Buton sıçramalarından kaynaklanan yanlış tetiklenmeleri önlemek için
 *          100ms debounce süresi uygular. Sadece debounce süresi geçtikten
 *          sonra basma işlemini kaydeder.
 */
bool button_pressed(uint gpio) {
    static absolute_time_t last_time;
    const uint32_t DEBOUNCE_US = 100000; // 100ms debounce süresi
    
    absolute_time_t current_time = get_absolute_time();
    
    if (gpio_get(gpio) && 
        (absolute_time_diff_us(last_time, current_time) > DEBOUNCE_US)) {
        last_time = current_time;
        return true;
    }
    return false;
}

/**
 * @brief Tüm buton GPIO pinlerini başlat
 * 
 * @details YUKARI, TAMAM ve AŞAĞI butonlarını pull-down dirençli giriş pinleri olarak
 *          yapılandırır. Bu, butonlar basılı değilken bilinen bir durumda
 *          olmalarını sağlar.
 */
void init_buttons(void) {
    // Buton pinlerini başlat
    gpio_init(BUTTON_UP);
    gpio_init(BUTTON_OK);
    gpio_init(BUTTON_DOWN);
    
    // Pull-down dirençli giriş olarak ayarla
    gpio_set_dir(BUTTON_UP, GPIO_IN);
    gpio_set_dir(BUTTON_OK, GPIO_IN);
    gpio_set_dir(BUTTON_DOWN, GPIO_IN);
    
    gpio_pull_down(BUTTON_UP);
    gpio_pull_down(BUTTON_OK);
    gpio_pull_down(BUTTON_DOWN);
}

/**
 * @brief GPIO pininden buton indeksini al
 * 
 * @param gpio GPIO pin numarası
 * @return int Buton indeksi (YUKARI için 0, TAMAM için 1, AŞAĞI için 2, geçersizse -1)
 * 
 * @details GPIO pin numaralarını buton durumu takibi için dizi indekslerine eşler.
 *          GPIO pini geçerli bir buton pini değilse -1 döndürür.
 */
static int get_button_index(uint gpio) {
    switch (gpio) {
        case BUTTON_UP: return 0;
        case BUTTON_OK: return 1;
        case BUTTON_DOWN: return 2;
        default: return -1;
    }
}

/**
 * @brief Çeşitli zamanlama kontrolleriyle buton basma kontrolü
 * 
 * @param gpio Kontrol edilecek GPIO pini
 * @param event Kontrol edilecek buton olayı türü (BUTTON_PRESSED, BUTTON_RELEASED veya BUTTON_HELD)
 * @return bool belirtilen olay gerçekleştiyse true
 * 
 * @details Farklı buton olay türlerini işler:
 *          - BUTTON_PRESSED: Buton ilk basıldığında bir kez tetiklenir
 *          - BUTTON_RELEASED: Buton bırakıldığında bir kez tetiklenir
 *          - BUTTON_HELD: Buton 1 saniyeden fazla basılı tutulduğunda tetiklenir
 *          
 * @note Debounce koruması içerir ve her buton için durum bilgisini saklar
 */
bool check_button_event(uint gpio, ButtonEvent event) {
    int idx = get_button_index(gpio);
    if (idx == -1) return false;
    
    absolute_time_t current_time = get_absolute_time();
    bool current_state = gpio_get(gpio);
    bool event_detected = false;
    
    // Buton durumunu güncelle
    if (current_state && !button_states[idx].is_pressed) {
        // Buton yeni basıldı
        if (absolute_time_diff_us(button_states[idx].last_press_time, current_time) > 200000) {
            button_states[idx].is_pressed = true;
            button_states[idx].last_press_time = current_time;
            button_states[idx].hold_duration = 0;
            
            if (event == BUTTON_PRESSED) {
                event_detected = true;
            }
        }
    } else if (!current_state && button_states[idx].is_pressed) {
        // Buton yeni bırakıldı
        button_states[idx].is_pressed = false;
        if (event == BUTTON_RELEASED) {
            event_detected = true;
        }
    }
    
    // Buton hala basılıysa basılı tutma süresini güncelle
    if (button_states[idx].is_pressed) {
        button_states[idx].hold_duration = 
            absolute_time_diff_us(button_states[idx].last_press_time, current_time) / 1000; // ms'ye çevir
        
        // Basılı tutma olayını kontrol et
        if (event == BUTTON_HELD && button_states[idx].hold_duration > 1000) { // 1 saniye basılı tutma
            event_detected = true;
        }
    }
    
    button_states[idx].was_pressed = button_states[idx].is_pressed;
    return event_detected;
}

/**
 * @brief Bir butonun ne kadar süre al
 * 
 * @param gpio Kontrol edilecek GPIO pini
 * @return uint32_t Basılı tutulma süresi (milisaniye cinsinden), basılı değilse 0
 * @details Butonun sürekli olarak ne kadar süre basılı tutulduğunu döndürür.
 *          Buton şu anda basılı değilse veya GPIO pini geçersizse 0 döndürür.
 */
uint32_t get_button_hold_duration(uint gpio) {
    int idx = get_button_index(gpio);
    if (idx == -1) return 0;
    
    if (button_states[idx].is_pressed) {
        return button_states[idx].hold_duration;
    }
    return 0;
}

/**
 * @brief Herhangi bir buton basılmasını bekle
 * 
 * @param timeout_ms Maksimum bekleme süresi (milisaniye cinsinden, sonsuz için 0)
 * @return uint Basılan butonun GPIO pini, veya zaman aşımında UINT_MAX
 * 
 * @details Şu durumlardan biri gerçekleşene kadar program akışını bloklar:
 *          1. Herhangi bir butona basılması (GPIO pin numarasını döndürür)
 *          2. Zaman aşımı süresinin dolması (UINT_MAX döndürür)
 *          3. timeout_ms 0 ise, sonsuza kadar bekler
 * 
 * @note Verimli bekleme için tight_loop_contents() makrosunu kullanır
 */
uint wait_for_button_press(uint32_t timeout_ms) {
    absolute_time_t start_time = get_absolute_time();
    
    while (true) {
        if (timeout_ms > 0 && 
            absolute_time_diff_us(start_time, get_absolute_time()) > timeout_ms * 1000) {
            return UINT_MAX;
        }
        
        if (button_pressed(BUTTON_UP)) return BUTTON_UP;
        if (button_pressed(BUTTON_OK)) return BUTTON_OK;
        if (button_pressed(BUTTON_DOWN)) return BUTTON_DOWN;
        
        tight_loop_contents();
    }
}
