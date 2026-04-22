/**
* @file    screen_flappy.c
* @brief   Flappy Bird clone with polished sprites.
*/

#include "ui_manager.h"
#include "app_display.h"
#include <stdlib.h>
#include <stdbool.h>

#define BIRD_X 30
#define BIRD_W 16
#define BIRD_H 12
#define PIPE_WIDTH 20
#define CAP_H 8
#define PIPE_GAP 45
#define GRAVITY 0.8f
#define JUMP_IMPULSE -3.5f

extern const AppScreen_t Screen_Games;

static float bird_y = 64.0f;
static float bird_vel = 0.0f;
static int pipe_x = 128;
static int pipe_gap_y = 40;
static int score = 0;
static bool game_over = false;

// Polished Bird Sprite (16x12)
static const uint8_t bird_sprite[] = {
    0x03, 0xE0, 0x0C, 0x10, 0x10, 0x08, 0x21, 0x04, 0x41, 0x02, 0x41, 0x02,
    0x41, 0xFE, 0x22, 0x02, 0x1C, 0x04, 0x03, 0xF8, 0x00, 0x00, 0x00, 0x00
};

static void Flappy_Init(void) {
    bird_y = 64.0f;
    bird_vel = 0.0f;
    pipe_x = 128;
    pipe_gap_y = 25 + (rand() % 45);
    score = 0;
    game_over = false;
}

static void Flappy_Draw(void) {
    app_display_clear();

    if (game_over) {
        char buf[16];
        app_display_draw_text_aligned(64, 30, 1, "GAME OVER", 0);
        app_display_draw_text_aligned(64, 50, 1, "SCORE:", 0);
        itoa(score, buf, 10);
        app_display_draw_text_aligned(64, 70, 1, buf, 0);
        app_display_draw_text_aligned(64, 100, 1, "CURTO: DENOVO", 0);
        app_display_update(DISPLAY_UPDATE_NORMAL);
        return;
    }

    // Draw Top Pipe
    app_display_draw_rect(pipe_x + 2, 0, PIPE_WIDTH - 4, pipe_gap_y - CAP_H, 0); // Body
    app_display_draw_rect(pipe_x, pipe_gap_y - CAP_H, PIPE_WIDTH, CAP_H, 0);    // Cap

    // Draw Bottom Pipe
    app_display_draw_rect(pipe_x, pipe_gap_y + PIPE_GAP, PIPE_WIDTH, CAP_H, 0); // Cap
    app_display_draw_rect(pipe_x + 2, pipe_gap_y + PIPE_GAP + CAP_H, PIPE_WIDTH - 4, 128 - (pipe_gap_y + PIPE_GAP + CAP_H), 0); // Body

    // Draw Bird Sprite
    app_display_draw_bitmap(BIRD_X, (int)bird_y, bird_sprite, BIRD_W, BIRD_H, 0);

    // Draw Score UI
    char score_buf[8];
    itoa(score, score_buf, 10);
    app_display_draw_text(60, 5, score_buf, 0);

    app_display_update(DISPLAY_UPDATE_PARTIAL);
}

static void Flappy_Update(void) {
    if (game_over) return;

    bird_vel += GRAVITY;
    bird_y += bird_vel;

    pipe_x -= 4;
    if (pipe_x < -PIPE_WIDTH) {
        pipe_x = 128;
        pipe_gap_y = 20 + (rand() % 55);
        score++;
    }

    // Collision
    if (bird_y < 0 || bird_y + BIRD_H > 127) game_over = true;

    if (BIRD_X + BIRD_W - 2 > pipe_x && BIRD_X + 2 < pipe_x + PIPE_WIDTH) {
        if (bird_y + 2 < pipe_gap_y || bird_y + BIRD_H - 2 > pipe_gap_y + PIPE_GAP) {
            game_over = true;
        }
    }
}

static void Flappy_OnEnter(void) {
    Flappy_Init();
    Flappy_Draw();
}

static void Flappy_OnEvent(UI_Event_t event) {
    if (event == EVENT_TICK_100MS) {
        Flappy_Update();
        Flappy_Draw();
    } 
    else if (event == EVENT_BTN_NEXT || event == EVENT_BTN_DOUBLE) {
        if (game_over) Flappy_Init();
        else bird_vel = JUMP_IMPULSE;
    }
    else if (event == EVENT_BTN_SELECT) {
        UI_Manager_SwitchScreen(&Screen_Games);
    }
}

const AppScreen_t Screen_Flappy = {
    .name = "Flappy",
    .on_enter = Flappy_OnEnter,
    .on_event = Flappy_OnEvent,
    .on_exit = NULL
};
