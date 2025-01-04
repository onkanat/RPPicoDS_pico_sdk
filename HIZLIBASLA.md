# Raspberry Pi Pico Eğitim Kartı - Hızlı Başlangıç

## Genel Bakış

Bu eğitim kartı, Raspberry Pi Pico ile C programlama ve Pico-SDK eğitimi için tasarlanmış 12 farklı komponent grubunu içerir. Her komponent grubu için gerekli pin tanımlamaları ve kullanım örnekleri aşağıda belirtilmiştir.

## Pin Grupları ve Fonksiyonları

### 1. Analog Girişler

- **Keypad (GPIO28)**
  - 3x4 matris keypad için analog okuma
  - Tuş takımı düzeni:

    ```markdown
    | 1 | 2 | 3 |
    | 4 | 5 | 6 |
    | 7 | 8 | 9 |
    | * | 0 | # |
    ```

  - `keypadOku()` fonksiyonu ile kontrol edilir
  - ADC değer aralıkları tanımlıdır

- **Potansiyometre (GPIO27)**
  - Analog değer okuma
  - 0-4095 arası ADC değeri
  - `read_analog()` fonksiyonu ile okunur

- **LDR Sensör (GPIO26)**
  - Işık seviyesi ölçümü
  - Analog değer okuma
  - `read_analog()` fonksiyonu ile okunur

### 2. Dijital Butonlar

- **Buton Grubu**
  - BUTTON_UP (GPIO5): Yukarı ok tuşu
  - BUTTON_OK (GPIO6): Onay/Enter tuşu
  - BUTTON_DOWN (GPIO7): Aşağı ok tuşu
  - Debounce korumalı okuma
  - `button_pressed()` ve `check_button_event()` fonksiyonları ile kontrol

### 3. LED Çıkışları

- **Basit LED'ler**
  - LED_RED (GPIO4): Kırmızı LED
  - LED_YELLOW (GPIO3): Sarı LED
  - LED_GREEN (GPIO2): Yeşil LED
  - Dijital çıkış veya PWM kontrolü

- **RGB LED (PWM Kontrollü)**
  - RGB_R (GPIO16): Kırmızı kanal
  - RGB_G (GPIO17): Yeşil kanal
  - RGB_B (GPIO18): Mavi kanal
  - PWM ile parlaklık kontrolü
  - `set_led_pwm()` fonksiyonu ile kontrol

### 4. Ses Çıkışı

- **Buzzer (GPIO10)**
  - PWM kontrollü ses üretimi
  - Nota çalma kapasitesi
  - `play_note()` ve `play_notes()` fonksiyonları
  - Frekans ve süre parametreleri ile kontrol

### 5. Step Motor

- **4-Pin Kontrol**
  - A1 (GPIO11)
  - A2 (GPIO12)
  - B1 (GPIO13)
  - B2 (GPIO14)
  - Tam adım ve yarım adım kontrol
  - `step_turn()` fonksiyonu ile kontrol

### 6. Sensör Modülleri

- **Ultrasonik Sensör**
  - TR (GPIO8): Trigger pin
  - EC (GPIO9): Echo pin
  - Mesafe ölçümü yapılır
  - `measure_distance()` fonksiyonu ile kontrol

- **PIR Sensör (GPIO19)**
  - Hareket algılama
  - Dijital giriş
  - `detect_motion()` fonksiyonu ile kontrol

### 7. Haberleşme Modülleri

- **Bluetooth**
  - TX (GPIO0)
  - RX (GPIO1)
  - UART haberleşme

- **I2C LCD Display**
  - SDA (GPIO20)
  - SCL (GPIO21)
  - Adres: 0x27
  - 2x16 karakter gösterim
  - `lcd_init()`, `lcd_clear()`, `lcd_string()` fonksiyonları

## Başlatma ve Konfigürasyon

Tüm komponentler `init_board()` fonksiyonu ile başlatılır. Bu fonksiyon:

- ADC başlatma
- PWM konfigürasyonu
- GPIO yönlendirmeleri
- I2C başlatma
- LCD başlatma işlemlerini gerçekleştirir.

## Kullanım Örnekleri

Her komponent grubu için örnek kullanımlar ve temel fonksiyonlar `pico_training_board.h` header dosyasında tanımlıdır. Detaylı implementasyonlar ilgili `.c` dosyalarında bulunmaktadır.

Bu hızlı başlangıç kılavuzu, Raspberry Pi Pico Eğitim Kartı ile çalışmaya başlamak için gerekli temel bilgileri ve örnekleri sunar. Daha fazla detay ve ileri seviye kullanım için ilgili dosyaları inceleyebilirsiniz.
