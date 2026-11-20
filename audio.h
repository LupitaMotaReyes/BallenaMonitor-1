// Audio de la pantalla: musica de fondo + efectos + voces, por el amplificador
// I2S integrado de la WT32-SC01 Plus (conector SPEAKER).
//
// Todo el audio corre en una tarea propia en el core 0, asi que dibujar la
// pantalla (core 1) no corta el sonido. Estas funciones solo encolan pedidos
// y se pueden llamar desde los eventos de LVGL.
//
// Hay 3 canales que suenan al mismo tiempo:
//   - musica : bgm en bucle
//   - efecto : correcto / incorrecto / burbuja
//   - voz    : ROJO / AMARILLO / VERDE / AZUL y felicitaciones (¡Bravo!, ...)
// La voz empieza en cuanto se pide (no espera a que termine el efecto); mientras
// suena, el efecto y la musica bajan de volumen para que se entienda la voz.

#pragma once

#include <stdint.h>

enum Sfx : uint8_t {
  SFX_CORRECT,
  SFX_WRONG,
  SFX_BUBBLE,
  SFX_ROJO,
  SFX_AMARILLO,
  SFX_VERDE,
  SFX_AZUL,
  // Felicitacion en RESUMEN DE SESSION (tambien van por el canal de voz)
  SFX_BRAVO,
  SFX_EXCELENTE,
  SFX_LO_LOGRASTE,
  SFX_BUEN_TRABAJO,
  SFX_MUY_BIEN,
  SFX_LO_HICISTE_GENIAL,
};

void audioBegin();

// Reproduce un sonido.
//   - Voces (SFX_ROJO..SFX_LO_HICISTE_GENIAL): suenan de inmediato en el canal de voz
//     (una voz nueva corta la anterior).
//   - SFX_CORRECT / SFX_WRONG: suenan de inmediato en el canal de efectos.
//   - SFX_BUBBLE: no corta a "correcto"/"incorrecto"; si uno de ellos esta
//     sonando, la burbuja se descarta.
void audioPlaySfx(Sfx sfx);

// Musica de fondo en bucle. Llamar con el mismo valor varias veces no hace nada.
void audioMusic(bool on);

// ---- Volumen ajustable desde la pantalla (boton de volumen) ----
// 0..100 %. 100 = volumen original. Se guarda en NVS con audioSaveVolume()
// y se recupera solo en audioBegin().
void audioSetVolume(uint8_t pct);        // general (voz + efectos + musica)
void audioSetMusicVolume(uint8_t pct);   // solo musica de fondo
uint8_t audioGetVolume();
uint8_t audioGetMusicVolume();
void audioSaveVolume();                  // llamar al soltar el slider (no en cada cambio)
