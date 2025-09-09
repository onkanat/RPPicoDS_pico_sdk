# LED ve RGB LED

\page howto_leds LED ve RGB LED

- PWM başlatma: `init_led_pwm()`, `init_rgb_pwm()`
- Tek LED: `set_led_pwm(gpio, duty)`
- RGB: `set_rgb_color(r, g, b)`

## Hızlı Başlangıç

```c
init_board();
init_rgb_pwm();
set_rgb_color(0, PWM_DUTY_MAX, 0); // yeşil
```

@see led_control.c
