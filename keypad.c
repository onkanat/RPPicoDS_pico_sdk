/**
 * @file keypad.c
 * @brief Pico Eğitim Kartı için tuş takımı okuma işlevselliği implementasyonu
 */

#include "pico_training_board.h"

/**
 * @brief Analog tuş takımını okur ve basılan tuşu döndürür
 * 
 * Bu fonksiyon, KEYPAD pinine (GPIO28) bağlı tuş takımından analog değeri okur
 * ve ADC değerini karşılık gelen tuş takımı karakterlerine eşleştirir.
 * Tuş takımı düğmeleri aşağıdaki matris şeklinde düzenlenmiştir:
 * | 1 | 2 | 3 |
 * | 4 | 5 | 6 |
 * | 7 | 8 | 9 |
 * | * | 0 | # |
 *
 * @return char Basılan tuşa karşılık gelen karakter, hiçbir tuşa basılmamışsa 0
 */
char keypadOku(void) {
    // Tuş takımı için ADC girişini seç (GPIO28)
    adc_select_input(2);
    
    // ADC değerini oku (12-bit çözünürlük: 0-4095)
    uint16_t deger = adc_read();
    
    // Hiçbir tuşa basılmadığını kontrol et
    if (deger > 0 && deger < 251) {
        return 0;
    }
    
    // ADC değerlerini tuş takımı karakterlerine eşle
    // Değerler verimli kontrol için azalan sırada düzenlenmiştir
    char key = 0;
    
    if      (deger > 2090) key = '1';
    else if (deger > 1385) key = '2';
    else if (deger > 735)  key = '3';
    else if (deger > 620)  key = '4';
    else if (deger > 574)  key = '5';
    else if (deger > 530)  key = '6';
    else if (deger > 401)  key = '7';
    else if (deger > 361)  key = '8';
    else if (deger > 340)  key = '9';
    else if (deger > 281)  key = '*';
    else if (deger > 261)  key = '0';
    else if (deger > 248)  key = '#';
    
    if (key != 0) {
        // Dokunsal geri bildirim için kısa bir bip sesi çal
        uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
        play_note(NOTE_C4, 25); // Önceden tanımlanmış nota frekansını kullan
    }
    
    return key;
}

/**
 * @brief Analog giriş kullanarak bir düğmeye basılıp basılmadığını kontrol eder
 * 
 * Bu fonksiyon, bir analog girişi okur ve ADC değerine göre bir düğmeye 
 * basılıp basılmadığını belirler ve debounce koruması içerir.
 *
 * @param gpio Okunacak GPIO pin numarası
 * @return bool düğmeye basılmışsa true, aksi halde false
 */
bool analog_button_pressed(uint gpio) {
    static absolute_time_t last_press_time = {0};
    const uint32_t DEBOUNCE_MS = 200;
    
    adc_select_input(gpio);
    uint16_t adc_value = adc_read();
    
    absolute_time_t current_time = get_absolute_time();
    
    // Son basmadan bu yana yeterli süre geçip geçmediğini kontrol et
    if (absolute_time_diff_us(last_press_time, current_time) > DEBOUNCE_MS * 1000) {
        // Düğme basma için ADC değer eşiği
        if (adc_value > 250) { // Eşik değeri gerektiğinde ayarlanabilir
            last_press_time = current_time;
            return true;
        }
    }
    
    return false;
}

/**
 * @brief Analog değeri okur
 * 
 * @param pin Okunacak ADC pin numarası
 * @return uint16_t Okunan analog değer (0-4095 arası)
 */
uint16_t read_analog(uint8_t pin) {
    // Verilen pin için ADC kanalını seç
    adc_select_input(pin);
    
    // 12-bit analog değeri oku (0-4095)
    uint16_t deger = adc_read();
    
    return deger;
}
