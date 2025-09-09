# Tuş Takımı (Analog)

\page howto_keypad Tuş Takımı (Analog)

- Okuma: `keypadOku()`
- Analog buton örneği: `analog_button_pressed(gpio)`

## Hızlı Başlangıç

```c
char k = keypadOku();
if (k) {
  lcd_clear();
  lcd_set_cursor(0,0);
  char msg[16];
  snprintf(msg, sizeof(msg), "Key:%c", k);
  lcd_string(msg);
}
```

@see keypad.c
