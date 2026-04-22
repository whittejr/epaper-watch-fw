/**
* @file    screen_snake.c
* @brief   Snake game using Button for control (Tilt logic preserved/commented)
*/

#include "ui_manager.h"
#include "app_display.h"
#include "app_accel.h"
#include <stdlib.h>
#include <stdbool.h>

#define GRID_SIZE 16
#define BLOCK_SIZE 8
#define MAX_SNAKE_LEN 64

extern const AppScreen_t Screen_Menu;

typedef struct {
    int8_t x, y;
} Point;

static Point snake[MAX_SNAKE_LEN];
static uint8_t snake_len = 3;
static Point food;
static int8_t dx = 1, dy = 0;
static bool game_over = false;

static void GenerateFood(void) {
    food.x = rand() % GRID_SIZE;
    food.y = rand() % GRID_SIZE;
}

static void Snake_Init(void) {
    snake_len = 3;
    snake[0] = (Point){8, 8};
    snake[1] = (Point){7, 8};
    snake[2] = (Point){6, 8};
    dx = 1; dy = 0;
    game_over = false;
    GenerateFood();
}

static void Snake_TurnClockwise(void) {
    // Current (dx, dy) -> New (dx, dy)
    // (1, 0)  [Right] -> (0, 1)  [Down]
    // (0, 1)  [Down]  -> (-1, 0) [Left]
    // (-1, 0) [Left]  -> (0, -1) [Up]
    // (0, -1) [Up]    -> (1, 0)  [Right]
    
    int8_t old_dx = dx;
    dx = -dy;
    dy = old_dx;
}

static void Snake_Draw(void) {
    app_display_clear();
    
    if (game_over) {
        app_display_draw_text_aligned(64, 40, 1, "GAME OVER", 0);
        app_display_draw_text_aligned(64, 70, 1, "CURTO: NOVO", 0);
        app_display_draw_text_aligned(64, 90, 1, "LONGO: VOLTAR", 0);
        app_display_update(DISPLAY_UPDATE_NORMAL);
        return;
    }

    // Draw Food
    app_display_draw_rect(food.x * BLOCK_SIZE, food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, 0);

    // Draw Snake
    for (int i = 0; i < snake_len; i++) {
        app_display_draw_rect(snake[i].x * BLOCK_SIZE, snake[i].y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, 0);
    }

    app_display_update(DISPLAY_UPDATE_NORMAL);
}


static void Snake_Update(void) {
    if (game_over) return;

    /* 
    // PRESERVED ACCELEROMETER LOGIC (Wait for readjustment request)
    float g[3];
    app_accel_read(g);
    if (g[0] < -0.3f && dx == 0) { dx = -1; dy = 0; }
    else if (g[0] > 0.3f && dx == 0) { dx = 1; dy = 0; }
    else if (g[1] < -0.3f && dy == 0) { dy = -1; dx = 0; }
    else if (g[1] > 0.3f && dy == 0) { dy = 1; dx = 0; }
    */

    // Move Body
    for (int i = snake_len - 1; i > 0; i--) {
        snake[i] = snake[i-1];
    }

    // Move Head
    snake[0].x += dx;
    snake[0].y += dy;

    // Boundaries
    if (snake[0].x < 0) snake[0].x = GRID_SIZE - 1;
    if (snake[0].x >= GRID_SIZE) snake[0].x = 0;
    if (snake[0].y < 0) snake[0].y = GRID_SIZE - 1;
    if (snake[0].y >= GRID_SIZE) snake[0].y = 0;

    // Collision with self
    for (int i = 1; i < snake_len; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            game_over = true;
        }
    }

    // Food
    if (snake[0].x == food.x && snake[0].y == food.y) {
        if (snake_len < MAX_SNAKE_LEN) snake_len++;
        GenerateFood();
    }
}

static void Snake_OnEnter(void) {
    Snake_Init();
    Snake_Draw();
}

static void Snake_OnEvent(UI_Event_t event) {
    if (event == EVENT_TICK_100MS) {
        static uint8_t game_tick = 0;
        game_tick++;
        if (game_tick >= 2) { // Game speed: 200ms
            game_tick = 0;
            Snake_Update();
            Snake_Draw();
        }
    } else if (event == EVENT_BTN_NEXT) {
        // Short Click: Turn 90 degrees clockwise or restart
        if (game_over) {
            Snake_Init();
            Snake_Draw();
        } else {
            Snake_TurnClockwise();
        }
    } else if (event == EVENT_BTN_SELECT) {
        // Long Click: Exit
        UI_Manager_SwitchScreen(&Screen_Menu);
    }
}

const AppScreen_t Screen_Snake = {
    .name = "Snake",
    .on_enter = Snake_OnEnter,
    .on_event = Snake_OnEvent,
    .on_exit = NULL
};
