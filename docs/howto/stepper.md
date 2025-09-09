# Step Motor

\page howto_stepper Step Motor

- Başlatma: `init_step_motor()` (gerekirse)
- Dönüş: `step_turn(direction, speed, revolutions)`
- Acil durdur: `step_stop()`
- Durum: `is_motor_running()`, `get_motor_state()`

## Core1 ile Kullanım (Önerilen)

```c
multicore_launch_core1(core1_main);
send_motor_parameters(CW, 900, 2.0f);
```

## Doğrudan Çağrı

```c
step_turn(CW, 600, 1.5f);
```

@warning `step_stop()` aniden durdurur; mekanik stres olabilir.

@see stepper.c
