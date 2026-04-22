/**
* @file    screen_temple.c
* @brief   Temple Run style game for 1-button watch
*/

#include "ui_manager.h"
#include "app_display.h"
#include <stdio.h>
#include <stdlib.h>

extern const AppScreen_t Screen_Games;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
#define LANE_WIDTH 30
#define NUM_LANES 3
#define HORIZON_Y 40

typedef struct {
    int8_t lane;
    float z; // Depth from 1.0 (horizon) to 0.0 (player)
    uint8_t active;
} Obstacle_t;

#define MAX_OBSTACLES 4
static Obstacle_t obstacles[MAX_OBSTACLES];
static int8_t player_lane = 1; // 0: Left, 1: Center, 2: Right
static uint16_t score = 0;
static uint8_t game_over = 0;
static float speed = 0.05f;

static void Temple_Reset(void) {
    player_lane = 1;
    score = 0;
    game_over = 0;
    speed = 0.05f;
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].active = 0;
    }
}

static void Temple_Draw(void) {
    app_display_clear();

    if (game_over) {
        app_display_draw_text_aligned(64, 40, 1, "TEMPLE FAILED", 0);
        char buf[16];
        snprintf(buf, sizeof(buf), "SCORE: %d", score);
        app_display_draw_text_aligned(64, 60, 1, buf, 0);
        app_display_draw_text_aligned(64, 100, 1, "CLIQUE: REVER", 0);
        app_display_draw_text_aligned(64, 115, 1, "LONGO: SAIR", 0);
    } else {
        // Draw Road/Path (Perspective lines)
        app_display_draw_rect(64-5, HORIZON_Y, 10, 1, 0); // Horizon line
        
        // Left line
        app_display_draw_rect(0, 127, 20, 1, 0);
        // Right line
        app_display_draw_rect(108, 127, 20, 1, 0);
        
        // Draw Obstacles
        for (int i = 0; i < MAX_OBSTACLES; i++) {
            if (obstacles[i].active) {
                // Perspective projection
                float scale = (1.0f - obstacles[i].z); // scale from 0 to 1
                int w = (int)(20 * scale);
                int h = (int)(15 * scale);
                if (w < 2) w = 2;
                if (h < 2) h = 2;
                
                int x_base = 64 + (obstacles[i].lane - 1) * (int)(40 * scale);
                int y = HORIZON_Y + (int)((SCREEN_HEIGHT - HORIZON_Y) * scale);
                
                app_display_draw_rect(x_base - w/2, y - h, w, h, 0);
            }
        }

        // Draw Player (Simple Triangle/Arrow)
        int px = 64 + (player_lane - 1) * 35;
        int py = 115;
        app_display_draw_rect(px - 5, py, 10, 5, 0);
        app_display_draw_rect(px - 2, py - 5, 4, 5, 0);

        // Score
        char s_buf[16];
        snprintf(s_buf, sizeof(s_buf), "SCORE: %d", score);
        app_display_draw_text(2, 2, s_buf, 0);
    }

    app_display_update(DISPLAY_UPDATE_PARTIAL);
}

static void Temple_OnEnter(void) {
    Temple_Reset();
    Temple_Draw();
}

static void Temple_OnEvent(UI_Event_t event) {
    if (event == EVENT_BTN_SELECT) {
        UI_Manager_SwitchScreen(&Screen_Games);
        return;
    }

    if (game_over) {
        if (event == EVENT_BTN_NEXT) {
            Temple_Reset();
            Temple_Draw();
        }
        return;
    }

    if (event == EVENT_TICK_100MS) {
        // Update Obstacles
        for (int i = 0; i < MAX_OBSTACLES; i++) {
            if (obstacles[i].active) {
                obstacles[i].z -= speed;
                if (obstacles[i].z <= 0) {
                    // Collision check
                    if (obstacles[i].lane == player_lane) {
                        game_over = 1;
                    } else {
                        obstacles[i].active = 0;
                        score++;
                        speed += 0.001f; // Speed up
                    }
                }
            } else {
                // Spawn new obstacle
                if (rand() % 20 == 0) {
                    obstacles[i].active = 1;
                    obstacles[i].z = 1.0f;
                    obstacles[i].lane = rand() % NUM_LANES;
                }
            }
        }
        Temple_Draw();
    } else if (event == EVENT_BTN_NEXT) {
        // Change lane (Cycle L -> C -> R -> L)
        player_lane++;
        if (player_lane >= NUM_LANES) player_lane = 0;
        Temple_Draw();
    }
}

const AppScreen_t Screen_Temple = {
    .name = "Temple Watch",
    .on_enter = Temple_OnEnter,
    .on_event = Temple_OnEvent,
    .on_exit = NULL
};
