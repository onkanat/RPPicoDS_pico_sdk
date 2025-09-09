# Butonlar

\page howto_buttons Butonlar

Bu sayfa, kart üzerindeki YUKARI, TAMAM ve AŞAĞI butonlarının kullanımını özetler.

- Başlatma: `init_buttons()`
- Olay kontrolü: `button_pressed(gpio)`, `check_button_event(gpio, event)`
- Bekleme: `wait_for_button_press(timeout_ms)`

## Hızlı Başlangıç

```c
#include "pico_training_board.h"

int main(void) {
  init_board();
  init_buttons();

  while (1) {
    if (button_pressed(BUTTON_UP)) {
      gpio_put(LED_GREEN, 1);
    }
    if (button_pressed(BUTTON_OK)) {
      gpio_put(LED_YELLOW, 1);
    }
    if (button_pressed(BUTTON_DOWN)) {
      gpio_put(LED_RED, 1);
    }
    tight_loop_contents();
  }
}
```

## IRQ Tabanlı Kullanım (Anti-Repeat)

`main.c` içinde örnek kesme geri çağrısı 50 ms anti-repeat kapısı uygular.

```c
void button_callback(uint gpio, uint32_t events) {
  static uint32_t last_irq_time_us; 
  uint32_t now = time_us_32();
  if ((now - last_irq_time_us) < 50000) return;
  last_irq_time_us = now;
  if (gpio == BUTTON_UP && button_pressed(BUTTON_UP)) { /* ... */ }
}
```

## Olay Türleri

`check_button_event(gpio, event)` ile:

- `BUTTON_PRESSED`: ilk basışta bir kez
- `BUTTON_RELEASED`: bırakıldığında bir kez
- `BUTTON_HELD`: >1 s basılı


## Bloklu Bekleme

```c
uint pressed = wait_for_button_press(3000);
if (pressed != UINT_MAX) {
  // buton basıldı
}
```

@see buttons.c
