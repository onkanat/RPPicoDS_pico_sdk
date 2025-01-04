/**
 * @file pico_training_board.h
 * @brief Raspberry Pi Pico eğitim kartı için pin tanımlamaları ve yapılandırmaları
 * @details Bu başlık dosyası, butonlar, LED'ler, LCD, motorlar ve sensörler dahil olmak üzere çeşitli bileşenler için yapılandırmalar ve fonksiyon prototipleri sağlar.
 * @author Hakan KILIÇASLAN
 * @date 2024-12-4
 */

#ifndef PICO_TRAINING_BOARD_H
#define PICO_TRAINING_BOARD_H

/* Standart kütüphaneler */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <limits.h>

/* Pico-SDK kütüphaneleri */
#include "hardware/gpio.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"

/**
 * @defgroup analog_inputs Analog Giriş Pin Tanımlamaları
 * @{
 */
#define KEYPAD 28 /**< Analog tuş takımı giriş pini */
#define POT_1 27  /**< Potansiyometre giriş pini */
#define LDR_1 26  /**< Işık sensörü giriş pini */
/** @} */

/**
 * @defgroup button_inputs Buton Giriş Pin Tanımlamaları
 * @{
 */
#define BUTTON_UP 5   /**< Yukarı ok butonu pini */
#define BUTTON_OK 6   /**< Tamam/Giriş butonu pini */
#define BUTTON_DOWN 7 /**< Aşağı ok butonu pini */
/** @} */

/**
 * @defgroup led_outputs LED Çıkış Pin Tanımlamaları
 * @{
 */
#define LED_RED 4    /**< Kırmızı LED */
#define LED_YELLOW 3 /**< Sarı LED */
#define LED_GREEN 2  /**< Yeşil LED */
#define RGB_R 16     /**< RGB LED Kırmızı (PWM) */
#define RGB_G 17     /**< RGB LED Yeşil (PWM) */
#define RGB_B 18     /**< RGB LED Mavi (PWM) */
/** @} */

/**
 * @details Buzzer kontrol pini
 */
#define BUZZER_PIN 10 /**< Buzzer çıkışı (PWM) */

/**
 * @defgroup stepper_motor Adım Motoru Tanımlamaları ve Kontrolleri
 * @{
 */
#define STEP_MOTOR_A1 11        /**< Adım motoru A1 pini */
#define STEP_MOTOR_A2 12        /**< Adım motoru A2 pini */
#define STEP_MOTOR_B1 13        /**< Adım motoru B1 pini */
#define STEP_MOTOR_B2 14        /**< Adım motoru B2 pini */
#define STEP_DELAY_MIN 1000     /**< Minimum adım gecikmesi (mikrosaniye) - Maksimum hız */
#define STEP_DELAY_MAX 10000    /**< Maksimum adım gecikmesi (mikrosaniye) - Minimum hız */
#define STEPS_PER_REV 2048      /**< Devir başına adım sayısı (28BYJ-48 motoru için) */
/** @} */

/**
 * @brief Motor durumu numaralandırması
 */
typedef enum {
    MOTOR_STOPPED = 0,  /**< Motor durmuş durumda */
    MOTOR_RUNNING = 1,  /**< Motor çalışıyor */
    MOTOR_ERROR = 2     /**< Hata durumu */
} motor_state_t;

/**
 * @brief Motor yönü numaralandırması
 */
typedef enum {
    CW,  /**< Saat yönünde */
    CCW  /**< Saat yönünün tersine */
} motor_direction_t;

// Motor kontrol fonksiyon prototipleri
bool is_motor_running(void);
motor_state_t get_motor_state(void);
void step_turn(motor_direction_t direction, uint speed, float revolutions);
void step_stop(void);
void init_step_motor(void);
void send_motor_parameters(motor_direction_t direction, uint speed, float revolutions);

// Ultrasonik Sensör
#define ULTRA_SONIC_TR 8 /**< Tetikleme pini */
#define ULTRA_SONIC_EC 9 /**< Yankı pini */

// Bluetooth Modülü
#define BLT_TX 0 /**< Bluetooth TX pini */
#define BLT_RX 1 /**< Bluetooth RX pini */

// PIR Sensör
#define PIR_DETECTOR 19 /**< PIR hareket sensörü pini */

// I2C LCD Ekran (Adres 0x27)
#define I2C_SDA 20 /**< I2C veri pini */
#define I2C_SCL 21 /**< I2C saat pini */
#define LCD_ADDR 0x27 /**< LCD I2C adresi */
#define I2C_PORT i2c0 /**< I2C portu */
#define LCD_ROWS 2 /**< LCD satır sayısı */
#define LCD_COLS 16 /**< LCD sütun sayısı */

// LCD için modlar
#define LCD_CHARACTER 1 /**< LCD karakter modu */
#define LCD_COMMAND 0 /**< LCD komut modu */

// LCD kontrol bayrakları
#define LCD_CLEARDISPLAY 0x01 /**< LCD ekranı temizle */
#define LCD_RETURNHOME 0x02 /**< LCD başlangıç konumuna dön */
#define LCD_ENTRYMODESET 0x04 /**< LCD giriş modu ayarla */
#define LCD_DISPLAYCONTROL 0x08 /**< LCD ekran kontrolü */
#define LCD_CURSORSHIFT 0x10 /**< LCD imleç kaydırma */
#define LCD_FUNCTIONSET 0x20 /**< LCD fonksiyon ayarı */
#define LCD_SETCGRAMADDR 0x40 /**< LCD CGRAM adres ayarı */
#define LCD_SETDDRAMADDR 0x80 /**< LCD DDRAM adres ayarı */

// LCD giriş modu bayrakları
#define LCD_ENTRYSHIFTINCREMENT 0x01 /**< LCD giriş kaydırma artışı */
#define LCD_ENTRYLEFT 0x02 /**< LCD giriş sola */

// LCD ekran kontrol bayrakları
#define LCD_BLINKON 0x01 /**< LCD yanıp sönme açık */
#define LCD_CURSORON 0x02 /**< LCD imleç açık */
#define LCD_DISPLAYON 0x04 /**< LCD ekran açık */

// LCD ekran/imleç kaydırma bayrakları
#define LCD_MOVERIGHT 0x04 /**< LCD sağa kaydır */
#define LCD_DISPLAYMOVE 0x08 /**< LCD ekran hareketi */

// LCD fonksiyon ayar bayrakları
#define LCD_5x10DOTS 0x04 /**< LCD 5x10 nokta */
#define LCD_2LINE 0x08 /**< LCD 2 satır */
#define LCD_8BITMODE 0x10 /**< LCD 8 bit modu */

// LCD arka ışık kontrolü
#define LCD_BACKLIGHT 0x08 /**< LCD arka ışık */
#define LCD_ENABLE_BIT 0x04 /**< LCD etkinleştirme biti */

// Genel tanımlar
#define HIGH 1 /**< Yüksek seviye */
#define LOW 0 /**< Düşük seviye */

// PWM tanımlamaları
#define PWM_FREQ 1000 /**< PWM frekansı */
#define PWM_DUTY_MAX 4095 /**< PWM maksimum görev döngüsü */

// Buzzer için PWM ayarları
#define BUZZER_CLOCK 125000000 /**< Buzzer saat frekansı (125 MHz) */
#define BUZZER_WRAP 4095 /**< Buzzer sarma değeri */
#define BUZZER_DUTY_50 2048 /**< Buzzer %50 görev döngüsü */

// Nota frekans tanımlamaları
#define NOTE_C4 261.63 /**< Do notası frekansı */
#define NOTE_CS4 277.18 /**< Do diyez notası frekansı */
#define NOTE_D4 293.66 /**< Re notası frekansı */
#define NOTE_DS4 311.13 /**< Re diyez notası frekansı */
#define NOTE_E4 329.63 /**< Mi notası frekansı */
#define NOTE_F4 349.23 /**< Fa notası frekansı */
#define NOTE_FS4 369.99 /**< Fa diyez notası frekansı */
#define NOTE_G4 392.00 /**< Sol notası frekansı */
#define NOTE_GS4 415.30 /**< Sol diyez notası frekansı */
#define NOTE_A4 440.00 /**< La notası frekansı */
#define NOTE_AS4 466.16 /**< La diyez notası frekansı */
#define NOTE_B4 493.88 /**< Si notası frekansı */

// Fonksiyon prototipleri
char keypadOku(void);
bool analog_button_pressed(uint gpio);

// Buton olay türleri
typedef enum
{
    BUTTON_PRESSED, /**< Buton basıldı */
    BUTTON_RELEASED, /**< Buton bırakıldı */
    BUTTON_HELD /**< Buton tutuldu */
} ButtonEvent;

// Buton fonksiyon prototipleri
bool button_pressed(uint gpio);
bool check_button_event(uint gpio, ButtonEvent event);
uint32_t get_button_hold_duration(uint gpio);
uint wait_for_button_press(uint32_t timeout_ms);

// Ek fonksiyon prototipleri
// PWM fonksiyon prototipleri
uint init_pwm_pin(uint gpio, float frequency, uint16_t duty);
void init_buzzer_pwm(void);
void init_rgb_pwm(void);
void init_led_pwm(void);
void init_board(void); // Tüm kart bileşenlerini başlat
void init_adc(void);   // ADC'yi başlat
void init_pwm(void);   // Tüm PWM'leri başlat
void init_lcd(void);   // LCD'yi başlat
void init_buttons(void);

uint16_t read_analog(uint8_t pin);

void set_led_pwm(uint gpio, uint16_t duty); // LED PWM görev döngüsünü ayarla
void set_pwm_frequency(uint slice_num, float frequency);
double get_frequency(const char *note, int octave);
void play_note(float frequency, int duration_ms);
void play_notes(const char *notes[][2], int num_notes, int duration);

// LCD fonksiyon prototipleri
void lcd_init(void);
void lcd_send_byte(uint8_t val, int mode);
void lcd_clear(void);
void lcd_set_cursor(int line, int position);
void lcd_string(const char *s);

// Sensör fonksiyon prototipleri
void init_ultrasonic(void);
void init_pir(void);
float measure_distance(void);
bool detect_motion(void);
bool wait_for_motion(uint32_t timeout_ms);
float get_average_distance(uint8_t num_samples, uint32_t delay_ms);

#endif // PICO_TRAINING_BOARD_H
