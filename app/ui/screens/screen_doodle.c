/**
* @file    screen_doodle.c
* @brief   Doodle Jump clone for 1-button watch
*/

#include "ui_manager.h"
#include "app_display.h"
#include "app_accel.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h> // Para uint8_t

extern const AppScreen_t Screen_Games;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

// --- AJUSTE: Tamanho do Jogador aumentado para mostrar detalhes ---
// O boneco é mais alto que largo.
#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 24

// --- AJUSTE: Plataforma e Física ---
// Aumentamos um pouco a plataforma para o jogador maior
#define PLATFORM_WIDTH 24 
#define NUM_PLATFORMS 8

// Física mantida, mas pode precisar de ajuste fino se parecer muito lento ou rápido.
// Vamos diminuir ligeiramente a gravidade e o salto para compensar o tamanho maior.
#define GRAVITY 0.45f
#define JUMP_VEL -7.5f
#define HORIZ_VEL 3.0f

typedef struct {
    float x, y;
} Platform_t;

static Platform_t platforms[NUM_PLATFORMS];
static float player_x, player_y;
static float player_vx, player_vy;
static float cam_y;
static uint32_t score;
static bool game_over;
static int anim_frame_counter = 0;

#include "seu_decio.h"

// --- DADOS DO SPRITE (BITMAPS) ---
// Estes são os arrays que representam o boneco em monocromático (16x24).
// Cada byte representa 8 pixels (1 bit por pixel).
// Assumimos que a API `app_display_draw_bitmap_pbm` desenha '1's em preto e '0's em branco.

static void app_display_draw_bitmap_pbm(int x, int y, int w, int h, const uint8_t *data, uint8_t color, bool flip_h) {
    int bytes_per_row = (w + 7) / 8;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int byte_idx = i * bytes_per_row + (j / 8);
            int bit_idx = 7 - (j % 8); // Assuming MSB first
            if (data[byte_idx] & (1 << bit_idx)) {
                int draw_x = flip_h ? (x + w - 1 - j) : (x + j);
                app_display_draw_rect(draw_x, y + i, 1, 1, color);
            }
        }
    }
}

static void Doodle_Reset(void) {
    player_x = 64;
    // --- AJUSTE: Posição Y inicial mais baixa para o boneco maior ---
    player_y = 50; 
    player_vx = HORIZ_VEL;
    player_vy = JUMP_VEL;
    cam_y = 0;
    score = 0;
    game_over = false;

    // Initial platforms
    platforms[0].x = (SCREEN_WIDTH - PLATFORM_WIDTH) / 2; // Centralizada
    platforms[0].y = 10;
    for (int i = 1; i < NUM_PLATFORMS; i++) {
        platforms[i].x = rand() % (SCREEN_WIDTH - PLATFORM_WIDTH);
        // --- AJUSTE: Mais espaçamento vertical para o boneco maior ---
        platforms[i].y = i * 45; 
    }
}

static void Doodle_Draw(void) {
    app_display_clear();

    if (game_over) {
        app_display_draw_text_aligned(64, 40, 1, "GAME OVER", 0);
        char buf[16];
        snprintf(buf, sizeof(buf), "SCORE: %lu", score);
        app_display_draw_text_aligned(64, 60, 1, buf, 0);
        app_display_draw_text_aligned(64, 100, 1, "CURTO: REVER", 0);
        app_display_draw_text_aligned(64, 115, 1, "LONGO: SAIR", 0);
    } else {
        // Draw Platforms
        for (int i = 0; i < NUM_PLATFORMS; i++) {
            int py = (int)(SCREEN_HEIGHT - (platforms[i].y - cam_y));
            if (py >= 0 && py < SCREEN_HEIGHT) {
                app_display_draw_rect((int)platforms[i].x, py, PLATFORM_WIDTH, 2, 0);
            }
        }

        // --- MUDANÇA: Desenhar o Player como Sprite ---
        int px = (int)player_x - DECIO_WIDTH / 2;
        int py = (int)(SCREEN_HEIGHT - (player_y - cam_y)) - DECIO_HEIGHT; 

        const uint8_t *sprite_to_draw;
        bool flip_h = false;

        // Animação simples baseada no movimento horizontal
        if (player_vx > 1.0f || player_vx < -1.0f) {
            anim_frame_counter++;
            // Alterna entre os frames de caminhada a cada 4 atualizações
            if ((anim_frame_counter / 4) % 2 == 0) {
                sprite_to_draw = decio_sprite_anda_1;
            } else {
                sprite_to_draw = decio_sprite_anda_2;
            }
            flip_h = (player_vx < -1.0f); 
        } else {
            // Parado
            sprite_to_draw = decio_sprite_parado;
            anim_frame_counter = 0;
        }

        app_display_draw_bitmap_pbm(px, py, DECIO_WIDTH, DECIO_HEIGHT, sprite_to_draw, 0, flip_h);

        // Draw Score
        char buf[16];
        snprintf(buf, sizeof(buf), "%lu", score);
        app_display_draw_text(2, 2, buf, 0);
    }

    app_display_update(DISPLAY_UPDATE_PARTIAL);
}

// --- Funções Auxiliares de Colisão (necessárias para o boneco maior) ---

static bool CheckCollisionWithPlatform(Platform_t p, float x, float y) {
    // Colisão mais rigorosa para o boneco maior: o centro do boneco precisa estar
    // sobre a plataforma, e o pé do boneco precisa estar no nível dela.
    
    // Profundidade do "pé" para encaixe
    float foot_depth = 5.0f;
    
    if (x + 2.0f > p.x && x - 2.0f < p.x + PLATFORM_WIDTH) { // Um pequeno "ponto central"
        // Verifica se o pé está na faixa de profundidade da plataforma
        if (y >= p.y && y <= p.y + foot_depth) {
            return true;
        }
    }
    return false;
}

static void Doodle_OnEnter(void) {
    Doodle_Reset();
    Doodle_Draw();
}

static void Doodle_OnEvent(UI_Event_t event) {
    if (event == EVENT_BTN_SELECT) {
        UI_Manager_SwitchScreen(&Screen_Games);
        return;
    }

    if (game_over) {
        if (event == EVENT_BTN_NEXT) {
            Doodle_Reset();
            Doodle_Draw();
        }
        return;
    }

    if (event == EVENT_TICK_50MS) {
        // Read accelerometer to set horizontal velocity
        float g[3];
        app_accel_get_last_g(g);
        
        // Amplifica o sinal (mais ágil)
        float target_vx = g[0] * 10.0f; 
        
        // Adiciona uma "deadzone"
        if (target_vx > -1.0f && target_vx < 1.0f) {
            target_vx = 0.0f;
        }
        
        // Suavização (Low-pass filter)
        player_vx = (player_vx * 0.7f) + (target_vx * 0.3f);
        
        // Limita a velocidade máxima
        if (player_vx > 8.0f) player_vx = 8.0f;
        if (player_vx < -8.0f) player_vx = -8.0f;
        
        // Physics
        player_vy += GRAVITY;
        player_y -= player_vy;
        player_x += player_vx;


        // Screen wrap horizontal
        if (player_x < 0) player_x = SCREEN_WIDTH;
        if (player_x > SCREEN_WIDTH) player_x = 0;

        // Camera follow
        // --- AJUSTE: Camera segue mais rápido e de mais baixo para o boneco maior ---
        if (player_y > cam_y + 90) {
            cam_y = player_y - 90;
        }

        // --- MUDANÇA: Colisão com plataformas usando a nova função auxiliar ---
        if (player_vy > 0) { // Only when falling
            for (int i = 0; i < NUM_PLATFORMS; i++) {
                if (CheckCollisionWithPlatform(platforms[i], player_x, player_y)) {
                    player_vy = JUMP_VEL;
                    if (platforms[i].y > score) score = (uint32_t)platforms[i].y;
                    break;
                }
            }
        }

        // Recycle platforms
        for (int i = 0; i < NUM_PLATFORMS; i++) {
            if (platforms[i].y < cam_y - 30) {
                // --- AJUSTE: Mais espaçamento vertical para o boneco maior ---
                platforms[i].y += NUM_PLATFORMS * 35 + (rand() % 25);
                platforms[i].x = rand() % (SCREEN_WIDTH - PLATFORM_WIDTH);
            }
        }

        // Game Over
        // --- AJUSTE: Mais tolerância para cair fora da tela ---
        if (player_y < cam_y - 30) {
            game_over = true;
        }

        Doodle_Draw();
    }
}

const AppScreen_t Screen_Doodle = {
    .name = "Seu Decio Jump",
    .on_enter = Doodle_OnEnter,
    .on_event = Doodle_OnEvent,
    .on_exit = NULL
};