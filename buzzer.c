/**
 * @file buzzer.c
 * @brief Pico Eğitim Kartı için buzzer nota çalma işlevselliği implementasyonu
 * @author Hakan KILIÇASLAN
 * @date 2024-12-4
 * Bu dosya, Pico Eğitim Kartı için buzzer ile ilgili fonksiyonların implementasyonunu,
 * nota frekans hesaplamalarını ve ses üretimi için PWM kontrolünü içerir.
 *
 * @see \ref howto_buzzer
 */

#include "pico_training_board.h"
#include <stdlib.h>

/**
 * @section usage Kullanım
 *
 * Basit bir melodi çalmak için nota ve oktav çiftlerinden oluşan bir dizi tanımlayıp
 * play_notes fonksiyonuna verin:
 *
 * @code{.c}
 * // Her eleman { nota, oktav } biçiminde dize çiftidir
 * static const char *MELODY[][2] = {
 *     {"C",  "4"},
 *     {"D#", "4"},
 *     {"F",  "4"}
 * };
 *
 * // Her notayı 250 ms çal
 * play_notes(MELODY, (int)(sizeof(MELODY) / sizeof(MELODY[0])), 250);
 * @endcode
 *
 * @note Desteklenen nota adlandırması doğal ve diyez notasyonudur (A, A#, B, C, C#, ...).
 *       Bemol için eşdeğer diyez notasyonu kullanılmalıdır (örn. Db yerine C#).
 */

/**
 * @struct NoteFrequency
 * @brief Müzikal nota bilgilerini saklayan yapı
 *
 * @var NoteFrequency::note
 * Müzikal notanın dize gösterimi
 * @var NoteFrequency::frequency
 * Notanın Hertz cinsinden frekans değeri
 */
typedef struct
{
    const char *note;  /**< Nota adı öneki (örn. "C", "C#") */
    double frequency;  /**< 4. oktavdaki temel frekans (Hz) */
} NoteFrequency;

/**
 * @brief 4. oktavdaki müzikal notalar ve karşılık gelen frekanslarının dizisi
 *
 * Bu tablo, frekans hesaplamaları için referans oktav olarak kullanılan
 * dördüncü oktavdaki müzikal notalar için standart frekansları içerir.
 */
static const NoteFrequency note_frequencies[] = {
    {"C4", 261.63},
    {"C#4/Db4", 277.18},
    {"D4", 293.66},
    {"D#4/Eb4", 311.13},
    {"E4", 329.63},
    {"F4", 349.23},
    {"F#4/Gb4", 369.99},
    {"G4", 392.00},
    {"G#4/Ab4", 415.30},
    {"A4", 440.00},
    {"A#4/Bb4", 466.16},
    {"B4", 493.88}};

/**
 * @brief Buzzer için özel olarak PWM'i başlatır
 *
 * Bu fonksiyon, buzzer'ın sessiz bir durumda başlamasını sağlamak için
 * başlangıç frekansı ve sıfır görev döngüsü ile buzzer için PWM pinini ayarlar.
 *
 * @note Bu fonksiyon, herhangi bir ses üretme girişiminden önce
 * sistem başlatılırken bir kez çağrılmalıdır.
 */
void init_buzzer_pwm(void)
{
    init_pwm_pin(BUZZER_PIN, PWM_FREQ, 0); // 0 görev döngüsü ile başlat
}

/**
 * @brief Belirtilen dilim için PWM frekansını ayarlar
 *
 * Bu fonksiyon, pico_training_board.h'den önceden tanımlanmış sabitleri kullanarak
 * istenen frekans için PWM dilimini yapılandırır. İstenen frekansa ulaşmak için
 * uygun bölücü değerlerini hesaplar.
 *
 * @param slice_num Yapılandırılacak PWM dilim numarası
 * @param frequency İstenen frekans (Hz cinsinden)
 *
 * @note Fonksiyon, istenen frekansa en yakın eşleşmeyi elde etmek için
 * bölücü değerlerini otomatik olarak ayarlar
 */
void set_pwm_frequency(uint slice_num, float frequency)
{
    uint32_t divider16 = (uint32_t)(BUZZER_CLOCK / frequency / BUZZER_WRAP);

    if (((uint32_t)(BUZZER_CLOCK / frequency)) % BUZZER_WRAP != 0)
    {
        divider16 += 1;
    }

    if (divider16 / 16 == 0)
    {
        divider16 = 16;
    }

    pwm_set_clkdiv_int_frac(slice_num, divider16 / 16, divider16 & 0xF);
    pwm_set_wrap(slice_num, BUZZER_WRAP);
    pwm_set_gpio_level(BUZZER_PIN, BUZZER_DUTY_50);
}

/**
 * @brief Adı ve oktavı verilen bir müzikal notanın frekansını hesaplar
 *
 * Bu fonksiyon, önceden tanımlanmış nota frekansları listesinden temel frekansını
 * bularak ve belirtilen oktava göre ayarlayarak, belirtilen müzikal notanın
 * frekansını belirler.
 *
 * @param note Müzikal notayı temsil eden dize (örn. "A", "C#", "G")
 * @param octave Oktav numarasını temsil eden tamsayı
 * @return Belirtilen oktavdaki notanın frekansı
 *
 * @note Referans oktav 4'tür ve diğer oktavlar için frekanslar
 * şu formül kullanılarak hesaplanır: f = f_temel * 2^(oktav_farkı)
 */
double get_frequency(const char *note, int octave)
{
    double base_frequency = 0.0;
    for (int i = 0; i < sizeof(note_frequencies) / sizeof(NoteFrequency); i++)
    {
        if (strncmp(note_frequencies[i].note, note, strlen(note)) == 0)
        {
            base_frequency = note_frequencies[i].frequency;
            break;
        }
    }
    int octave_difference = octave - 4;
    return base_frequency * pow(2, octave_difference);
}

/**
 * @brief Belirtilen frekans ve sürede buzzer'da bir nota çalar
 *
 * Bu fonksiyon, PWM kullanarak bir nota çalar ve nota çalma işlemi
 * tamamlandıktan sonra uygun temizleme işlemlerini gerçekleştirir.
 * Parametre doğrulaması içerir ve notalar arası temiz geçişleri sağlar.
 *
 * @param frequency Çalınacak notanın Hertz cinsinden frekansı
 * @param duration_ms Notanın çalınması gereken süre (milisaniye cinsinden)
 *
 * @note Fonksiyon, notaların birbirine karışmasını önlemek ve temiz ses üretimini
 * sağlamak için her notadan sonra küçük bir gecikme içerir
 */
void play_note(float frequency, int duration_ms)
{
    if (frequency <= 0)
    {
        return; // Geçersiz parametreler
    }

    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    set_pwm_frequency(slice_num, frequency);
    sleep_ms(duration_ms);
    pwm_set_gpio_level(BUZZER_PIN, 0);
    sleep_us(50); // Notaların birbirine karışmasını önlemek için küçük gecikme
}
/**
 * @brief Bir dizi müzikal notanın çalınmasını sağlar
 *
 * Bu fonksiyon, bir dizi notanın üzerinden yineleyerek her bir nota için
 * frekansı hesaplar ve belirtilen sürede çalar.
 *
 * @param notes  Her elemanı {nota, oktav} olan iki boyutlu dize dizisi.
 *               Örn. {"C","4"}, {"D#","4"}
 * @param num_notes Dizideki nota sayısı
 * @param duration Her bir notanın çalma süresi (ms)
 *
 * @note Nota adı karşılaştırması önekle yapılır; diyez notasyonu kullanın ("Db" yerine "C#").
 * @code{.c}
 * static const char *MELODY[][2] = {{"C","4"},{"E","4"},{"G","4"}};
 * play_notes(MELODY, (int)(sizeof(MELODY)/sizeof(MELODY[0])), 200);
 * @endcode
 */
void play_notes(const char *notes[][2], int num_notes, int duration) {
    for (int i = 0; i < num_notes; i++) {
        const char *note = notes[i][0];
        int octave = atoi(notes[i][1]);
        double frequency = get_frequency(note, octave);
        printf("Playing frequency: %.2f Hz for %d ms\n", frequency, duration);
        play_note(frequency, duration);
    }
}
