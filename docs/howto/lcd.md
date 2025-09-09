# LCD (I2C)

\page howto_lcd LCD (I2C)

- Başlatma: `init_lcd()` veya doğrudan `lcd_init()` (projede `init_board()` içinde çağrılır)
- Metin yazma: `lcd_string(const char *s)`
- İmleç: `lcd_set_cursor(line, pos)`
- Temizleme: `lcd_clear()`

## Hızlı Başlangıç

```c
init_board();
lcd_clear();
lcd_set_cursor(0, 0);
lcd_string("Merhaba");
lcd_set_cursor(1, 0);
lcd_string("Pico LCD");
```

@see lcd_i2c.c
