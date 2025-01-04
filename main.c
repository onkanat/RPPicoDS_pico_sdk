/**
 * @file main.c
 * @brief Ana program, adım motoru kontrolü için Raspberry Pi Pico Eğitim Kartı'nı başlatır.
 * @author Hakan KILIÇASLAN
 * @date 2024-02-07
 */

#include "pico_training_board.h"

/**
 * @brief 
 */
static volatile bool motor_running = false;
static volatile bool motor_command_pending = false;
static volatile motor_direction_t current_direction = CW;
static volatile motor_direction_t requested_direction = CW;

/**
 * @brief Button interrupt callback function
 */
//FIXME: Birden fazla komut geliyor !!!!
void button_callback(uint gpio, uint32_t events)
{
    if (gpio == BUTTON_UP && button_pressed(BUTTON_UP))
    {
        gpio_put(LED_GREEN, HIGH);
        gpio_put(LED_YELLOW, LOW);
        send_motor_parameters(CW, 900, 100.0f);
    }
    else if (gpio == BUTTON_DOWN && button_pressed(BUTTON_DOWN))
    {
        gpio_put(LED_RED, HIGH);
        gpio_put(LED_YELLOW, LOW);
        send_motor_parameters(CCW, 900, 300.0f);
    }
    else if (gpio == BUTTON_OK && button_pressed(BUTTON_OK))
    {
        gpio_put(LED_GREEN, LOW);
        gpio_put(LED_RED, LOW);
        gpio_put(LED_YELLOW, HIGH);
    }
}

/**
 * @brief Initialize button interrupts for motor control
 */
//FIXME: buton callbak değişecek birden fazla komut gidiyor !!!
void init_motor_buttons(void)
{
    // Configure button GPIO interrupts
    gpio_set_irq_enabled_with_callback(BUTTON_UP,
                                       GPIO_IRQ_EDGE_RISE, // Only rising edge
                                       true,
                                       &button_callback);

    gpio_set_irq_enabled_with_callback(BUTTON_DOWN,
                                       GPIO_IRQ_EDGE_RISE, // Only rising edge
                                       true,
                                       &button_callback);

    gpio_set_irq_enabled_with_callback(BUTTON_OK,
                                       GPIO_IRQ_EDGE_RISE, // Only rising edge
                                       true,
                                       &button_callback);
}

/**
 * @brief Core 1'de çalışacak işlev
 *
 * Bu fonksiyon RP2040'ın Core 1'inde çalışır ve multicore FIFO üzerinden Core 0'dan
 * gelen adım motoru kontrol komutlarını işler. 
 * Üç parametre işler: 
 * yön, hız ve devir sayısı.
 *
 * Fonksiyon:
 * 1. Core 0'dan gelen verileri bekler
 * 2. FIFO'dan motor parametrelerini okur
 * 3. Motor hareketini yürütür
 * 4. Core 0'a tamamlanma sinyalini geri gönderir
 *
 * @note Bu fonksiyon sonsuz bir döngüde çalışır ve Core 0'ın verileri doğru
 *       sırada göndermesini gerektirir: yön, hız, devir sayısı
 *
 * @see step_turn()
 * @see multicore_fifo_pop_blocking()
 * @see multicore_fifo_push_blocking()
 */
void core1_main()
{
    while (true)
    {
        // Wait for data from Core 0
        if (multicore_fifo_rvalid())
        {
            // Read motor parameters from FIFO
            uint32_t direction = multicore_fifo_pop_blocking(); // Direction parameter
            uint32_t speed = multicore_fifo_pop_blocking();     // Speed parameter
            uint32_t temp = multicore_fifo_pop_blocking();
            float revolutions = *(float *)&(temp); // Convert uint32_t to float for revolutions

            // Execute motor movement
            step_turn((motor_direction_t)direction, speed, revolutions);

            // Send completion signal back to Core 0
            multicore_fifo_push_blocking(0xDEAD); // Completion signal
        }
    }
}

float revolutions;

/**
 * @brief Step motor parametrelerini Core1'e gönderir
 *
 * Bu fonksiyon, step motor kontrolü için gerekli parametreleri Core0'dan Core1'e
 * multicore FIFO üzerinden gönderir. Gönderilen parametreler:
 * 1. Motor dönüş yönü
 * 2. Motor hızı
 * 3. Devir sayısı
 *
 * @param direction Motor dönüş yönü (CW: saat yönü, CCW: saat yönü tersi)
 * @param speed Motor hızı (adım/saniye)
 * @param revolutions Yapılacak devir sayısı
 * 
 * @note Fonksiyon, her parametre için FIFO'ya bloklu gönderim yapar ve
 *       Core1'in verileri almasını bekler
 * 
 * @see core1_main() Core1'de çalışan ve bu parametreleri alan fonksiyon
 * @see step_turn() Gönderilen parametrelerle motoru kontrol eden fonksiyon
 */
void send_motor_parameters(motor_direction_t direction, uint speed, float revolutions)
{
    // Core1'e veri gönder
    multicore_fifo_push_blocking((uint32_t)direction);
    multicore_fifo_push_blocking(speed);
    multicore_fifo_push_blocking(*(uint32_t *)&revolutions);
}

// LCD ekrana veri yazan fonksiyon
void write_analog_to_lcd(char *component_name, float value)
{
    // Bildiri mesajını hazırla
    char message[32];
    snprintf(message, sizeof(message), "%s: %.f", component_name, value);
    // LCD ekrana veri yaz

    lcd_string(message);
    sleep_ms(100);
}

// Keypad analog girişini LCD'e yazan fonksiyon
void display_keypad_value()
{

    int key = keypadOku();
    if (key != 0)
    {
        lcd_clear();
        lcd_set_cursor(0, 0);
        char ascii_key;
        ascii_key = (char)key;
        char message[16];
        snprintf(message, sizeof(message), "Keypad:%c", ascii_key);
        lcd_string(message);
        // write_analog_to_lcd("Keypad", key);
        sleep_ms(300);
    }
}

// Potansiyometre analog girişini LCD'e yazan fonksiyon
void display_potentiometer_value()
{

    lcd_set_cursor(0, 0);
    float pot_value = read_analog(1);
    write_analog_to_lcd("POT_1", pot_value);
}

// LDR sensör analog girişini LCD'e yazan fonksiyon
void display_ldr_sensor_value()
{

    lcd_set_cursor(1, 0);
    float light_level = read_analog(0);
    write_analog_to_lcd("LDR_1", light_level);
}

/**
 *  Ana program, adım motoru kontrolü için Raspberry Pi Pico Eğitim Kartı'nı başlatır.
 *
 *  Bu program, adım motorunu kontrol etmek için gerekli olan 
 *  ilklendirmeleri gerçekleştirir ve ana döngüde 
 *  kullanıcıdan veya geri çağırma işlevlerinden veri alarak 
 *  adım motoru kontrolünü sağlar.
 *
 *  @return 0 Program başarıyla tamamlandığında.
 */
int main()
{
    // Initialize system
    init_board();
    init_motor_buttons();

    gpio_init(LED_GREEN);
    gpio_init(LED_YELLOW);
    gpio_init(LED_RED);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_set_dir(LED_RED, GPIO_OUT);


    // Core1 başlat
    multicore_launch_core1(core1_main);

    int counter = 0;
    char buffer[16];

    while (true)
    {
        // Kullanıcıdan veya bir callback fonksiyonundan veri al
        // Example usage
        lcd_clear();
        lcd_set_cursor(1, 13);
        sprintf(buffer, "%d", counter++);
        lcd_string(buffer);
        sleep_ms(1000);

        // İş tamamlanmasını bekle
        while (!multicore_fifo_rvalid())
        {

            display_keypad_value();
            display_ldr_sensor_value();
            display_potentiometer_value();
            tight_loop_contents(); // Bekleme sırasında işlemciyi serbest bırak
            
            lcd_set_cursor(0, 11);
            sprintf(buffer, "C:%d", counter++);
            lcd_string(buffer);
            sleep_ms(1000);
        }
        uint32_t result = multicore_fifo_pop_blocking();
        if (result == 0xDEAD)
        {
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_RED, 0);
            gpio_put(LED_YELLOW, 1);
            printf("step_turn işlemi tamamlandı.\n");
        }

        tight_loop_contents(); // Bekleme sırasında işlemciyi serbest bırak
    }
    

}
