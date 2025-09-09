# Tasks: Pico Eğitim Kartı – Doxygen ve Küçük İyileştirmeler

Input: /specs/001-pico-e-itim/plan.md
Prerequisites: plan.md (required). No contracts/data-model for embedded; focus on docs and small feature work.

## Format: [ID] [P?] Description

- [P]: Paralel çalışabilir (farklı dosyalar, bağımsız)
- Her görev açıklamasında tam dosya yollarını verin

## Phase 3.1: Setup

- [ ] T001 [P] Doxygen ve Graphviz kurulumunu doğrula; yoksa kur (macOS): doxygen, graphviz. Çıktı: `docs/html/index.html`
- [ ] T002 [P] Doxyfile INPUT ve FILE_PATTERNS doğrula; `INPUT` boşsa proje kökünü ve `*.c, *.h, *.md` kapsayacak şekilde güncelle: `/Users/hakankilicaslan/pico/RPPicoDS_pico_sdk/Doxyfile`
- [ ] T003 [P] README.md “Dokümantasyon Üretimi” bölümüne sorun giderme notlarını ekle (Graphviz/dot uyarıları, OUTPUT_LANGUAGE notu): `/Users/hakankilicaslan/pico/RPPicoDS_pico_sdk/README.md`

## Phase 3.2: Tests First (Doc QA as gates)

- [ ] T004 [P] Doxygen uyarılarını sıfıra yaklaştır: tüm public API’lerde eksik `@param/@return` ve hatalı void etiketlerini düzelt (buttons.c, buzzer.c, keypad.c, lcd_i2c.c, led_control.c, sensors.c, stepper.c, init_functions.c, main.c)
- [ ] T005 [P] Her kaynak dosyaya `@file` bloğu ekle ve kısa açıklama yaz: aynı dosyalar
- [ ] T006 [P] Doxygen’de çağrı grafikleri için `HAVE_DOT=YES` ve ilişkili ayarları doğrula: `/Users/hakankilicaslan/pico/RPPicoDS_pico_sdk/Doxyfile`

## Phase 3.3: Core Implementation

- [ ] T007 [P] Buton IRQ debouncing: `buttons.c` içinde IRQ callback’e minimum aralık (örn. 30–50 ms) “gating” ekle; son tetik zamanı microsecond counter ile tutulacak. Header’da dokümante et: `/Users/hakankilicaslan/pico/RPPicoDS_pico_sdk/buttons.c`
- [ ] T008 [P] Buzzer API dokümantasyonunu tamamla; örnek melodi tanımı açıklaması ekle: `/Users/hakankilicaslan/pico/RPPicoDS_pico_sdk/buzzer.c`
- [ ] T009 [P] LCD I2C kısa kullanım rehberi için fonksiyonlara örnek satır ek açıklamalar (@code blokları): `/Users/hakankilicaslan/pico/RPPicoDS_pico_sdk/lcd_i2c.c`
- [ ] T010 [P] Sensors modülünde ultrasonik/PIR/pot/LDR fonksiyonları için aralıklar ve hata notları (@note) ekle: `/Users/hakankilicaslan/pico/RPPicoDS_pico_sdk/sensors.c`
- [ ] T011 [P] Step motor API’de çalışma/durma durum sorguları için örnek kullanım (@code) ve zamanlama uyarıları: `/Users/hakankilicaslan/pico/RPPicoDS_pico_sdk/stepper.c`
- [ ] T012 [P] keypad.c ve led_control.c fonksiyonlarına parametre açıklamaları ve basit örnekler ekle: ilgili dosyalar

## Phase 3.4: Integration (Docs site polish)

- [ ] T013 [P] “Nasıl Kullanılır” kısa sayfaları oluştur: `/docs/howto/` altında `buttons.md, lcd.md, buzzer.md, sensors.md, stepper.md, keypad.md, leds.md`; her biri için 5–10 satırlık özet + pin haritasına atıf. Doxygen INPUT’a bu klasörü ekle.
- [ ] T014 [P] README.md’ye “Kalite Kapıları” bölümü ekle: Build, Doxygen, Uyarı eşikleri ve sık hatalar: `/Users/hakankilicaslan/pico/RPPicoDS_pico_sdk/README.md`

## Phase 3.5: Polish

- [ ] T015 [P] Doxygen’i temiz çalıştır: `docs/` temizle, yeniden üret; kalan uyarıları listele ve gider.
- [ ] T016 [P] VS Code Tasks notlarını README’de güncelle (Compile/Run/Flash açıklama ve sorun giderme): `/Users/hakankilicaslan/pico/RPPicoDS_pico_sdk/README.md`
- [ ] T017 [P] Dokümanlarda modül sayfalarına “Nasıl Kullanılır” bağlantıları eklemek için ilgili dosya üstüne @see linkleri ekle.

## Dependencies

- T001, T002, T006 → T004, T005 için ön-koşul (doküman altyapısı)
- T004, T005 → T013, T017 (dokümantasyon içeriği)
- T007 bağımsız, ancak dokümantasyonu için T004/T005 ile birlikte gözden geçirilmeli
- T013 → T015 (site temiz üretim)

## Parallel Example

- Aynı anda çalıştırılabilir: T001, T002, T003, T006
- İçerik işleri paralel: T008–T012

## Validation Checklist

- [ ] ./docs/html/index.html güncellenmiş ve call/caller grafikleri var
- [ ] Public API’lerde eksik `@param/@return` yok; void imzalarında yanlış etiket yok
- [ ] buttons.c’de IRQ debouncing uygulanmış ve yorumlarda açıklanmış
- [ ] /docs/howto/*.md sayfaları mevcut ve Doxygen’de listeleniyor
- [ ] README’de kalite kapıları ve görevler güncel
