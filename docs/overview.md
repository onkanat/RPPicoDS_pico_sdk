# Proje Genel Bakış

\mainpage Pico Eğitim Kartı — Genel Bakış

Bu dokümantasyon, Raspberry Pi Pico tabanlı Eğitim Kartı’nı ve örnek kodları kapsar.

- Hızlı başlangıç: `init_board()` tüm alt sistemleri başlatır.
- Kaynak kodlar: `.c` ve `.h` dosyaları proje kökünde bulunur.

## Hızlı Erişim

- Nasıl Kullanılır İçindekiler → \ref howto_index
	- Butonlar → \ref howto_buttons
	- LCD (I2C) → \ref howto_lcd
	- Buzzer → \ref howto_buzzer
	- Sensörler (PIR + Ultrasonik) → \ref howto_sensors
	- Step Motor → \ref howto_stepper
	- Tuş Takımı (Analog) → \ref howto_keypad
	- LED ve RGB LED → \ref howto_leds

## İçerik

- Donanım bileşenleri: butonlar, LED/RGB, LCD (I2C), buzzer, tuş takımı, sensörler, step motor.
- Çok çekirdekli kullanım: Core1 ile step motor kontrolü (FIFO).
- Doxygen grafikleri için Graphviz (dot) gereklidir.

> Not: Türkçe çıktı bazı sabit metinlerde İngilizce içerebilir (Doxygen dil dosyası uyarısı).
