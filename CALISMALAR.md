
# Raspberry Pi Pico Eğitim Kartı - Örnek Çalışmalar

## Örnek Programlar

### 1. Basit Mesaj Gösterme

```c
int main() {
    // I2C ve LCD başlatma işlemleri...
    
    lcd_clear();
    lcd_string("Merhaba Dünya!");
    sleep_ms(2000);
    
    lcd_clear();
    lcd_string("Raspberry Pi");
    lcd_set_cursor(1, 0);
    lcd_string("Pico");
}
```

### 2. Değişken Değer Gösterme

```c
void display_temperature(float temp) {
    char buffer[16];
    lcd_clear();
    lcd_string("Sicaklik:");
    lcd_set_cursor(1, 0);
    sprintf(buffer, "%.1f C", temp);
    lcd_string(buffer);
}
```

### 3. Menü Gösterimi

```c
void show_menu_item(const char* item, int selected) {
    lcd_clear();
    if (selected) {
        lcd_string("> ");
    } else {
        lcd_string("  ");
    }
    lcd_string(item);
}
```

### 4. Otomatik Pozisyonlama

```c
void position_control(float target_position) {
    float current_position = 0;
    while(current_position < target_position) {
        step_turn(CW, 50, 0.1);  // 0.1 tur adımlarla hareket
        current_position += 0.1;
    }
}
```

### 5. Hız Profili

```c
void speed_profile(void) {
    // Yavaşça hızlan
    for(int i = 10; i <= 100; i += 10) {
        step_turn(CW, i, 0.5);
    }
    
    // Yavaşça yavaşla
    for(int i = 90; i >= 10; i -= 10) {
        step_turn(CW, i, 0.5);
    }
}
```

### 6. Sayaç

```c
void display_counter(void) {
    int counter = 0;
    char buffer[16];
    
    while(1) {
        lcd_clear();
        lcd_string("Sayac:");
        lcd_set_cursor(1, 0);
        sprintf(buffer, "%d", counter++);
        lcd_string(buffer);
        sleep_ms(1000);
    }
}
```

