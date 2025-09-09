/**
 * @file led_control.c
 * @brief Pico Eğitim Kartı için led ve rgb kontrolü uygulaması
 * @see \ref howto_leds
 */
#include "pico_training_board.h"

/**
 * @brief RGB LED pinleri için PWM'i başlatır
 * Tüm üç RGB LED kanalını PWM ile yapılandırır
 */
void init_rgb_pwm(void) {
    // Her RGB kanalını başlat
    init_pwm_pin(RGB_R, PWM_FREQ, 0);
    init_pwm_pin(RGB_G, PWM_FREQ, 0);
    init_pwm_pin(RGB_B, PWM_FREQ, 0);
}

/**
 * @brief Standart LED çıkışları için PWM'i başlatır
 * Kırmızı, Sarı ve Yeşil LED'ler için PWM'i yapılandırır
 */
void init_led_pwm(void) {
    // Standart LED'leri başlat
    init_pwm_pin(LED_RED, PWM_FREQ, 0);
    init_pwm_pin(LED_YELLOW, PWM_FREQ, 0);
    init_pwm_pin(LED_GREEN, PWM_FREQ, 0);
}

/**
 * @brief Bir LED için PWM görev döngüsünü ayarlar
 * 
 * @param gpio LED'in GPIO pin numarası
 * @param duty Görev döngüsü değeri (0-PWM_DUTY_MAX)
 */
void set_led_pwm(uint gpio, uint16_t duty) {
    if (duty > PWM_DUTY_MAX) {
        duty = PWM_DUTY_MAX;
    }
    pwm_set_gpio_level(gpio, duty);
}

/**
 * @brief PWM kullanarak RGB LED rengini ayarlar
 * 
 * @param red Kırmızı bileşen (0-PWM_DUTY_MAX)
 * @param green Yeşil bileşen (0-PWM_DUTY_MAX)
 * @param blue Mavi bileşen (0-PWM_DUTY_MAX)
 *
 * @code{.c}
 * init_rgb_pwm();
 * set_rgb_color(0, PWM_DUTY_MAX, 0); // Yeşil
 * @endcode
 */
void set_rgb_color(uint16_t red, uint16_t green, uint16_t blue) {
    set_led_pwm(RGB_R, red);
    set_led_pwm(RGB_G, green);
    set_led_pwm(RGB_B, blue);
}
