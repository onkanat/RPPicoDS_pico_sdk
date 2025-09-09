# Raspberry Pi Pico Eğitim Kartı

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
  - BUTTON_UP (**GPIO5**): Yukarı ok tuşu
  - BUTTON_OK (**GPIO6**): Onay/Enter tuşu
  - BUTTON_DOWN (**GPIO7**): Aşağı ok tuşu
  - Debounce korumalı okuma
  - `button_pressed()` ve `check_button_event()` fonksiyonları ile kontrol

### 3. LED Çıkışları

- **Basit LED'ler**
  - LED_RED (**GPIO4**): Kırmızı LED
  - LED_YELLOW (**GPIO3**): Sarı LED
  - LED_GREEN (**GPIO2**): Yeşil LED
  - Dijital çıkış veya PWM kontrolü

- **RGB LED (PWM Kontrollü)**
  - RGB_R (**GPIO16**): Kırmızı kanal
  - RGB_G (**GPIO17**): Yeşil kanal
  - RGB_B (**GPIO18**): Mavi kanal
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
  - A1 (**GPIO11**)
  - A2 (**GPIO12**)
  - B1 (**GPIO13**)
  - B2 (**GPIO14**)
  - Tam adım ve yarım adım kontrol
  - `step_turn()` fonksiyonu ile kontrol

### 6. Sensör Modülleri

- **Ultrasonik Sensör**
  - TR (**GPIO8**): Trigger pin
  - EC (**GPIO9**): Echo pin
  - Mesafe ölçümü yapılır
  - `measure_distance()` fonksiyonu ile kontrol

- **PIR Sensör (GPIO19)**
  - Hareket algılama
  - Dijital giriş
  - `detect_motion()` fonksiyonu ile kontrol

### 7. Haberleşme Modülleri

- **Bluetooth**
  - TX (**GPIO0**)
  - RX (**GPIO1**)
  - UART haberleşme

- **I2C LCD Display**
  - SDA (**GPIO20**)
  - SCL (**GPIO21**)
  - Adres: **0x27**
  - 2x16 karakter gösterim
  - `lcd_init()`, `lcd_clear()`, `lcd_string()` fonksiyonları

## Başlatma ve Konfigürasyon

Tüm komponentler `init_board()` fonksiyonu ile başlatılır. Bu fonksiyon:

- ADC başlatma
- PWM konfigürasyonu
- GPIO yönlendirmeleri
- I2C başlatma
- LCD başlatma işlemlerini gerçekleştirir.

## Pin Özeti

```c
// Analog inputs
#define KEYPAD 28        // Analog giriş
#define POT_1 27         // Analog giriş
#define LDR_1 26         // Analog giriş

// Buttons inputs
#define BUTTON_UP 5      // Digital input (Arrow Up)
#define BUTTON_OK 6      // Digital input Arrow OK
#define BUTTON_DOWN 7    // Digital input Arrow Down

// LED outputs
#define LED_RED 4        // Digital output Red led
#define LED_YELLOW 3     // Digital output Yellow led
#define LED_GREEN 2      // Digital output Green led
#define RGB_R 16         // Digital output (PWM) red
#define RGB_G 17         // Digital output (PWM) green
#define RGB_B 18         // Digital output (PWM) blue

// Sound
#define BUZZER_PIN 10    // Digital output Alarm

// Step motor module
#define STEP_MOTOR_A1 11
#define STEP_MOTOR_A2 12
#define STEP_MOTOR_B1 13
#define STEP_MOTOR_B2 14

// Ultra sonic sensor module
#define ULTRA_SONIC_TR 8 // TR time
#define ULTRA_SONIC_EC 9 // EC time

// Bluetooth module
#define BLT_TX 0
#define BLT_RX 1

// PIR module
#define PIR_DETECTOR 19

// LCD Display I2C (Adres: 0x27)
#define SDA 20
#define SCL 21
```

Bu yeni `README.md` dosyası, `pico_training_board.h` dosyasındaki güncel verileri ve diğer dokümanlardaki bilgileri birleştirerek kapsamlı bir rehber sunmaktadır.

## Dokümantasyon Üretimi (Doxygen)

Bu proje Doxygen ile belgelenmiştir. HTML ve LaTeX çıktıları `./docs` klasörüne üretilir.

Gereksinimler:

- Doxygen (1.9+ önerilir)
- Graphviz (dot) — çağrı/çağıran grafikleri için

Adımlar (macOS):

```bash
# Graphviz yoksa kurun
brew install graphviz

# Doxygen’i çalıştırın (repo kökünde)
doxygen Doxyfile
```

Çıktılar:

- HTML: `docs/html/index.html`
- LaTeX: `docs/latex/`

Notlar:

- Doxygen Türkçe dil dosyası eski uyarısı görülebilir; işlevi etkilemez.
- Dot kaynaklı hatalar varsa önce `docs/html` ve `docs/latex` dizinlerini silip tekrar çalıştırın.

## Sorun Giderme (Doxygen)

- Graphviz/dot bulunamadı uyarısı veya grafikler üretilmiyor:
  - macOS için Graphviz kurulu olmalı ve `dot -V` çalışmalı.
  - Doxyfile içinde `HAVE_DOT = YES`, `CALL_GRAPH = YES`, `CALLER_GRAPH = YES` olmalı.
  - Gerekirse docs’u temizleyip yeniden üretin.
- OUTPUT_LANGUAGE Turkish uyarısı:
  - Doxygen Türkçe dil paketi eski olabilir; bu sadece bazı sabit metinlerin İngilizce görünmesine yol açar. İşlevselliği etkilemez.

## Nasıl Kullanılır (How-To)

Modül bazlı hızlı başlangıç rehberleri için Doxygen içinde şu bölüme bakın:

- Nasıl Kullanılır İçindekiler: howto_index
  - Butonlar, LCD, Buzzer, Sensörler, Step Motor, Tuş Takımı, LED/RGB

## Kalite Kapıları

- Build: Ninja ile proje derlenmeli (VS Code görevi: Compile Project).
- Docs: Doxygen çalışmalı, Graphviz grafiklerini üretmeli.
- Uyarılar: Yalnızca Türkçe dil uyarısı kabul edilebilir; diğer uyarılar çözülmeli.
