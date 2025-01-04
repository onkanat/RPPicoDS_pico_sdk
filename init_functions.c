/**
 * @file init_functions.c
 * @brief Pico Eğitim Kartı için başlatma fonksiyonlarının implementasyonu
 */

#include "pico_training_board.h"

/**
 * @brief Analog girişler için ADC'yi başlatır
 * 
 * Aşağıdakiler için ADC'yi yapılandırır ve GPIO pinlerini başlatır:
 * - Tuş takımı (GPIO28)
 * - Potansiyometre (GPIO27)
 * - Işık sensörü (GPIO26)
 */
void init_adc(void) {
    adc_init();
    adc_gpio_init(LDR_1);    // Işık sensörü 0
    adc_gpio_init(POT_1);    // Potansiyometre 1
    adc_gpio_init(KEYPAD);   // Tuş takımı girişi 2
}

/**
 * @brief Buzzer ve LED çıkışları için PWM'i başlatır
 * 
 * Aşağıdakiler için PWM kanallarını yapılandırır:
 * - Buzzer
 * - RGB LED çıkışları
 */
void init_pwm(void) {
    // Buzzer PWM'ini başlat
    init_buzzer_pwm();
    init_rgb_pwm();
    init_led_pwm();
}

/**
 * @brief Belirli bir GPIO pini için PWM'i başlatır
 *
 * @param gpio PWM için başlatılacak GPIO pin numarası
 * @param frequency Başlangıç PWM frekansı Hz cinsinden (uygulanabilirse, varsayılan için 0)
 * @param duty Başlangıç görev döngüsü (0-PWM_DUTY_MAX)
 * @return uint Yapılandırılmış PWM dilim numarası
 */
uint init_pwm_pin(uint gpio, float frequency, uint16_t duty) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    
    // Varsayılan PWM yapılandırmasını al
    pwm_config config = pwm_get_default_config();
    
    if (frequency > 0) {
        // İstenen frekans için saat bölücüyü hesapla
        float divider = (float)BUZZER_CLOCK / (frequency * PWM_DUTY_MAX);
        pwm_config_set_clkdiv(&config, divider);
    }
    
    // Sarma değerini ayarla
    pwm_config_set_wrap(&config, PWM_DUTY_MAX);
    
    // PWM'i yapılandırma ile başlat
    pwm_init(slice_num, &config, true);
    
    // Başlangıç görev döngüsünü ayarla
    pwm_set_gpio_level(gpio, duty);
    
    return slice_num;
}

/**
 * @brief I2C LCD ekranını başlatır
 * 
 * I2C iletişimini yapılandırır ve LCD ekranını
 * standart 16x2 yapılandırması ile başlatır
 */
void init_lcd(void) {
    // LCD için I2C'yi başlat
    i2c_init(I2C_PORT, 100 * 1000);  // 100 kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    // LCD başlatma sırası
    sleep_ms(100);  // LCD'nin güç alması için bekle
    
    // LCD 4-bit başlatma sırası
    lcd_send_byte(0x03, LCD_COMMAND);
    sleep_ms(5);
    lcd_send_byte(0x03, LCD_COMMAND);
    sleep_us(100);
    lcd_send_byte(0x03, LCD_COMMAND);
    sleep_us(100);
    lcd_send_byte(0x02, LCD_COMMAND);  // 4-bit moda ayarla
    
    // LCD'yi yapılandır
    lcd_send_byte(0x28, LCD_COMMAND);  // 2 satır, 5x8 yazı tipi
    lcd_send_byte(0x0C, LCD_COMMAND);  // Ekran AÇIK, imleç KAPALI
    lcd_send_byte(0x06, LCD_COMMAND);  // İmleç otomatik artır
    lcd_send_byte(0x01, LCD_COMMAND);  // Ekranı temizle
    
    sleep_ms(2);  // Ekranı temizleme için daha uzun gecikme gerekli
}

/**
 * @brief Tüm kart bileşenlerini başlatır
 * 
 * Diğer tüm başlatma fonksiyonlarını çağıran ve butonlar ve LED'ler için
 * GPIO pinlerini ayarlayan ana başlatma fonksiyonu
 */
void init_board(void) {
    // stdio'yu başlat
    stdio_init_all();
    
    // Tüm alt sistemleri başlat
    init_adc();
    init_pwm();
    init_lcd();
    init_buzzer_pwm();
    init_buttons();
    init_ultrasonic();
    init_pir(); // Yeni eklenen fonksiyon çağrısı
    
    // Basit LED'ler
    gpio_init(LED_RED);
    gpio_init(LED_YELLOW);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    
    // Tüm çıkışları bilinen duruma başlat
    gpio_put(LED_RED, LOW);
    gpio_put(LED_YELLOW, LOW);
    gpio_put(LED_GREEN, LOW);
    gpio_put(RGB_B, LOW);
    gpio_put(RGB_G, LOW);
    gpio_put(RGB_R, LOW);
    
    // Tüm bileşenlerin hazır olduğundan emin olmak için küçük bir gecikme
    sleep_ms(100);
}
