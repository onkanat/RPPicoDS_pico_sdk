# Buzzer

\page howto_buzzer Buzzer

- Başlatma: `init_buzzer_pwm()` (projede `init_board()` çağırır)
- Tek nota: `play_note(frequency, duration_ms)`
- Melodi: `play_notes(const char *notes[][2], int num_notes, int duration_ms)`

## Frekansla Çalma

```c
init_board();
play_note(440.0f, 300); // A4, 300 ms
```

## Nota + Oktav ile Çalma

```c
static const char *MELODY[][2] = {{"C","4"},{"E","4"},{"G","4"}};
play_notes(MELODY, (int)(sizeof(MELODY)/sizeof(MELODY[0])), 200);
```

@note Nota adında diyez kullanın (Db yerine C#).

@see buzzer.c
