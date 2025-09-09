# Sensörler (PIR + Ultrasonik)

\page howto_sensors Sensörler (PIR + Ultrasonik)

- PIR başlatma: `init_pir()` / Okuma: `detect_motion()`
- Ultrasonik başlatma: `init_ultrasonic()` / Mesafe: `measure_distance()`
- Zaman aşımlı bekleme: `wait_for_motion(timeout_ms)`
- Ortalama mesafe: `get_average_distance(n, delay_ms)`

## Hızlı Başlangıç

```c
init_board();
if (wait_for_motion(5000)) {
  float d = measure_distance();
  // d < 0 ise ölçüm başarısız
}
```

## Ortalama Alma

```c
float avg = get_average_distance(5, 50);
```

@note `measure_distance()` dönüşü -1.0 ise zaman aşımı demektir.

@see sensors.c
