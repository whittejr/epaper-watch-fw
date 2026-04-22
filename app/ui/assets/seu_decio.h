/**
 * @file seu_decio.h
 * @brief Sprites do Seu Decio para display monocromático (1bpp, MSB-first)
 * Dimensões: 16x24 pixels (48 bytes por frame)
 */

#ifndef SEU_DECIO_H
#define SEU_DECIO_H

#include <stdint.h>

#define DECIO_WIDTH  16
#define DECIO_HEIGHT 24

// Frame 1: Parado de frente
extern const uint8_t decio_sprite_parado[48];

// Frame 2: Caminhando (perna direita levantada)
extern const uint8_t decio_sprite_anda_1[48];

// Frame 3: Caminhando (perna esquerda levantada)
extern const uint8_t decio_sprite_anda_2[48];

#endif // SEU_DECIO_H
