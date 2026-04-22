/**
* @file    screen_parkour.c
* @brief   Parkour runner (Vector style) for 1-button watch.
*/

#include "ui_manager.h"
#include "app_display.h"
#include <stdlib.h>
#include <stdbool.h>

#define GROUND_Y 110
#define PLAYER_X 20
#define OBSTACLE_SPEED 6

extern const AppScreen_t Screen_Games;

typedef enum { RUN, JUMP, SLIDE, FALL } State_t;

static float player_y = GROUND_Y;
static float player_vel = 0;
static State_t player_state = RUN;
static int obs_x = 140;
static int obs_type = 0; // 0: Pequeno (pular), 1: Alto (deslizar), 2: Buraco
static int score = 0;
static bool game_over = false;
static uint8_t anim_frame = 0;

// Sprites de Silhueta (16x16)
static const uint8_t runner_run1[] = { 0x06, 0x00, 0x06, 0x00, 0x0F, 0x00, 0x1F, 0x00, 0x0E, 0x00, 0x0E, 0x00, 0x1B, 0x00, 0x1B, 0x00, 0x31, 0x80, 0x31, 0x80, 0x60, 0xC0, 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const uint8_t runner_run2[] = { 0x06, 0x00, 0x06, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x0E, 0x00, 0x0E, 0x00, 0x0E, 0x00, 0x0E, 0x00, 0x0E, 0x00, 0x1F, 0x00, 0x31, 0x80, 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const uint8_t runner_jump[] = { 0x06, 0x00, 0x0F, 0x00, 0x1F, 0x00, 0x0E, 0x00, 0x0E, 0x00, 0x1F, 0x00, 0x31, 0x80, 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const uint8_t runner_slide[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x3F, 0xFC, 0x7F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static void Parkour_Init(void) {
    player_y = GROUND_Y - 16;
    player_vel = 0;
    player_state = RUN;
    obs_x = 140;
    obs_type = rand() % 3;
    score = 0;
    game_over = false;
}

static void Parkour_Draw(void) {
    app_display_clear();

    if (game_over) {
        char buf[16];
        app_display_draw_text_aligned(64, 40, 1, "WASTED", 0);
        app_display_draw_text_aligned(64, 60, 1, "SCORE:", 0);
        itoa(score, buf, 10);
        app_display_draw_text_aligned(64, 80, 1, buf, 0);
        app_display_draw_text_aligned(64, 105, 1, "CURTO: RESTART", 0);
        app_display_update(DISPLAY_UPDATE_NORMAL);
        return;
    }

    // Desenha o "Prédio" (Chão)
    app_display_draw_rect(0, GROUND_Y, 128, 128 - GROUND_Y, 0);

    // Desenha Obstáculos
    if (obs_type == 0) { // Caixote (Pular)
        app_display_draw_rect(obs_x, GROUND_Y - 12, 12, 12, 0);
    } else if (obs_type == 1) { // Viga Alta (Slide)
        app_display_draw_rect(obs_x, GROUND_Y - 25, 6, 15, 0);
    } else if (obs_type == 2) { // Buraco
        // Limpa uma parte do chão para simular gap
        app_display_draw_rect(obs_x, GROUND_Y, 30, 20, 1); 
    }

    // Desenha Player
    const uint8_t *sprite;
    if (player_state == JUMP) sprite = runner_jump;
    else if (player_state == SLIDE) sprite = runner_slide;
    else sprite = (anim_frame % 2 == 0) ? runner_run1 : runner_run2;

    app_display_draw_bitmap(PLAYER_X, (int)player_y, sprite, 16, 16, 0);

    // Score
    char s_buf[12];
    itoa(score, s_buf, 10);
    app_display_draw_text(110, 5, s_buf, 0);

    app_display_update(DISPLAY_UPDATE_PARTIAL);
}

static void Parkour_Update(void) {
    if (game_over) return;

    anim_frame++;

    // Física de Pulo
    if (player_state == JUMP) {
        player_vel += 1.2f; // Gravidade
        player_y += player_vel;
        if (player_y >= GROUND_Y - 16) {
            player_y = GROUND_Y - 16;
            player_state = RUN;
        }
    } else if (player_state == SLIDE) {
        static uint8_t slide_timer = 0;
        slide_timer++;
        if (slide_timer > 5) {
            slide_timer = 0;
            player_state = RUN;
        }
    }

    // Movimento do mundo
    obs_x -= OBSTACLE_SPEED;
    if (obs_x < -30) {
        obs_x = 140;
        obs_type = rand() % 3;
        score++;
    }

    // Colisões
    if (player_state != JUMP) {
        // Colisão com caixote
        if (obs_type == 0 && obs_x < PLAYER_X + 12 && obs_x > PLAYER_X) game_over = true;
        // Colisão com buraco
        if (obs_type == 2 && obs_x < PLAYER_X + 10 && obs_x > PLAYER_X - 10) game_over = true;
    }
    
    // Colisão com viga alta (só morre se não estiver em SLIDE)
    if (obs_type == 1 && obs_x < PLAYER_X + 12 && obs_x > PLAYER_X) {
        if (player_state != SLIDE) game_over = true;
    }
}

static void Parkour_OnEnter(void) {
    Parkour_Init();
    Parkour_Draw();
}

static void Parkour_OnEvent(UI_Event_t event) {
    if (event == EVENT_TICK_100MS) {
        Parkour_Update();
        Parkour_Draw();
    } 
    else if (event == EVENT_BTN_NEXT) {
        if (game_over) Parkour_Init();
        else if (player_state == RUN) {
            player_state = JUMP;
            player_vel = -6.0f;
        }
    }
    else if (event == EVENT_BTN_DOUBLE) {
        if (player_state == RUN) {
            player_state = SLIDE;
        }
    }
    else if (event == EVENT_BTN_SELECT) {
        UI_Manager_SwitchScreen(&Screen_Games);
    }
}

const AppScreen_t Screen_Parkour = {
    .name = "Parkour",
    .on_enter = Parkour_OnEnter,
    .on_event = Parkour_OnEvent,
    .on_exit = NULL
};
