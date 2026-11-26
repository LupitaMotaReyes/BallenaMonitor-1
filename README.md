# Ballena / Tortuga - pantalla WT32-SC01 Plus

UI de SquareLine Studio (LVGL 8.3.11) que controla la tortuga por ESP-NOW.
El receptor (XIAO ESP32-C6 + motor) está en el repo `tortuga-motor-test`.

## Compilar y subir

Requiere, en la carpeta de librerías de Arduino (`Documents/Arduino/libraries`):

- `lvgl` **8.3.11** y `LovyanGFX`
- `lv_conf.h` → copiar `extras/lv_conf.h` al lado de la carpeta `lvgl`
  (color 16 bits sin swap, Montserrat 16/20/26, `LV_TICK_CUSTOM 1` con `millis()`).

```sh
arduino-cli compile -b esp32:esp32:wt32-sc01-plus -u -p COM11 .
```

## ESP-NOW

- `motor_protocol.h` debe ser idéntico al del repo `tortuga-motor-test`.
- `turtleAddress` = MAC del XIAO (la imprime su Serial al arrancar).
- Mientras una flecha está presionada se reenvía el comando cada 100 ms; al soltarla se manda STOP.

## EMG (directo en la pantalla)

La pantalla lee el circuito EMG y filtra la señal ella misma, sin el ESP32 aparte
(`EMG_REMOTE = false` en `emg.h`).

| Circuito EMG | WT32-SC01 Plus (conector Extended IO) |
|---|---|
| Salida (0–3.1 V) | GPIO10 |
| GND | GND |
| Alimentación | 3V3 |

Usa GPIO10: es el único pin de ADC1 del conector, y los de ADC2 (GPIO11–14) no funcionan con Wi-Fi/ESP-NOW.
Proceso, a 1 kHz: notch 60 Hz → pasa-altas 20 Hz → pasa-bajas 300 Hz → rechazo de artefactos →
RMS 150 ms → suavizado → umbral con histéresis. Comandos por Serial (115200): `p` = plotter,
`c` = calibrar, `t30` = umbral 30 mV, `?` = estado.

**Pila de 9 V de la ballena** (`BALLENA_BAT_PIN = 11` en `emg.h`), divisor con 4 resistencias de 62k:

```
+ pila ── 62k ── 62k ── 62k ──┬── GPIO11
                              └── 62k ── GND ── − pila
```

GPIO11 es ADC2, así que algunas lecturas fallan con el Wi-Fi encendido: se ignoran y se conserva el último valor bueno.
Con `?` en el Serial se ve el voltaje medido.

Para volver al ESP32 aparte (`EMG_Emisor/`), cambia a `EMG_REMOTE = true`.

## Nube de colores (MODO JUEGO)

La nube está dibujada dentro del fondo `ui_img_4_png.c`. Su relleno es una máscara aparte
(`ui_img_nube.c`, formato ALPHA_8BIT) que se pinta con `img_recolor` en `generateRandomColor()`.
Si cambias el fondo en SquareLine, regenera la máscara:

```sh
python tools/make_nube_mask.py
```
