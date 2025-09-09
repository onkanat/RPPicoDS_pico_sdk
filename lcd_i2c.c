/**
 * @file lcd_i2c.c
 * @brief I2C iletişimi kullanan LCD kontrol fonksiyonları
 * @see \ref howto_lcd
 */

#include "pico_training_board.h"

// Varsayılan I2C LCD adresi
static int addr = LCD_ADDR;

/**
 * @brief I2C LCD'ye tek byte veri yazar
 *
 * @param val Yazılacak byte değeri
 */
static void i2c_write_byte(uint8_t val) {
    i2c_write_blocking(I2C_PORT, addr, &val, 1, false);
}

/**
 * @brief LCD'nin enable pinini değiştirir
 *
 * @param val Mevcut byte değeri
 */
static void lcd_toggle_enable(uint8_t val) {
    const int DELAY_US = 600;
    sleep_us(DELAY_US);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

/**
 * @brief LCD'ye bir byte veri gönderir
 *
 * @param val Gönderilecek byte
 * @param mode Komutlar için LCD_COMMAND, karakter verisi için LCD_CHARACTER
 */
void lcd_send_byte(uint8_t val, int mode) {
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

/**
 * @brief LCD ekranını temizler
 */
void lcd_clear(void) {
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
}

/**
 * @brief LCD imlecini belirtilen konuma ayarlar
 *
 * @param line Satır numarası (0 veya 1)
 * @param position Karakter pozisyonu (0-15)
 */
void lcd_set_cursor(int line, int position) {
    int val = (line == 0) ? 0x80 + position : 0xC0 + position;
    lcd_send_byte(val, LCD_COMMAND);
}

/**
 * @brief LCD'ye bir karakter gönderir
 *
 * @param val Gönderilecek karakter
 */
static void lcd_char(char val) {
    lcd_send_byte(val, LCD_CHARACTER);
}

/**
 * @brief LCD'ye bir metin dizisi gönderir
 *
 * @param s Null ile sonlandırılmış gönderilecek metin
 */
void lcd_string(const char *s) {
    while (*s) {
        lcd_char(*s++);
    }
}

/**
 * @brief LCD ekranını başlatır
 * 
 * LCD ekranını 4-bit mod, 2 satır ve 5x8 yazı tipi ile yapılandırır.
 * Ayrıca ekranı temizler ve imleci başlangıç konumuna getirir.
 */
void lcd_init(void) {
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x02, LCD_COMMAND);

    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
    lcd_clear();
}

/**
 * @section lcd_usage LCD Kullanım Örneği
 *
 * @code{.c}
 * // I2C yapılandırması proje genelinde yapılmış varsayılır
 * lcd_init();
 * lcd_set_cursor(0, 0);
 * lcd_string("Merhaba");
 * lcd_set_cursor(1, 0);
 * lcd_string("Pico LCD");
 * @endcode
 *
 * @note LCD_ADDR, I2C_PORT ve pin konfigürasyonları `pico_training_board.h` içinde tanımlıdır.
 */
