/**
* @file    screen_doom.c
* @brief   Doom Lite with Enemy and Shooting (Double Click)
*/

#include "ui_manager.h"
#include "app_display.h"
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAP_SIZE 8
#define FOV 0.66f
#define STEP_SIZE 0.15f
#define ROT_SPEED 1.5708f // 90 degrees

extern const AppScreen_t Screen_Menu;

static const uint8_t world_map[MAP_SIZE][MAP_SIZE] = {
    {1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,1},
    {1,0,1,0,0,1,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,0,1},
    {1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1}
};

typedef struct {
    float x, y;
    bool active;
} Sprite_t;

static float posX = 1.5f, posY = 1.5f;
static float dirX = 1.0f, dirY = 0.0f;
static float planeX = 0.0f, planeY = 0.66f;

static Sprite_t enemy = {4.5f, 4.5f, true};
static uint8_t shoot_flash = 0;
static float z_buffer[128]; // Store wall distances for sprite occlusion

static void Doom_Draw(void) {
    app_display_clear();

    // 1. Raycasting Walls
    for (int x = 0; x < 128; x += 2) {
        float cameraX = 2.0f * x / 128.0f - 1.0f;
        float rayDirX = dirX + planeX * cameraX;
        float rayDirY = dirY + planeY * cameraX;

        int mapX = (int)posX;
        int mapY = (int)posY;

        float sideDistX, sideDistY;
        float deltaDistX = fabsf(1.0f / rayDirX);
        float deltaDistY = fabsf(1.0f / rayDirY);
        float perpWallDist;

        int stepX, stepY;
        int hit = 0, side;

        if (rayDirX < 0) { stepX = -1; sideDistX = (posX - mapX) * deltaDistX; }
        else { stepX = 1; sideDistX = (mapX + 1.0f - posX) * deltaDistX; }
        if (rayDirY < 0) { stepY = -1; sideDistY = (posY - mapY) * deltaDistY; }
        else { stepY = 1; sideDistY = (mapY + 1.0f - posY) * deltaDistY; }

        while (hit == 0) {
            if (sideDistX < sideDistY) { sideDistX += deltaDistX; mapX += stepX; side = 0; }
            else { sideDistY += deltaDistY; mapY += stepY; side = 1; }
            if (world_map[mapX][mapY] > 0) hit = 1;
        }

        if (side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
        else           perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

        z_buffer[x] = perpWallDist;
        z_buffer[x+1] = perpWallDist;

        int lineHeight = (int)(128 / perpWallDist);
        int drawStart = -lineHeight / 2 + 64;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + 64;
        if (drawEnd >= 128) drawEnd = 127;

        app_display_draw_rect(x, drawStart, 2, drawEnd - drawStart, 0);
    }

    // 2. Sprite Rendering (Enemy)
    if (enemy.active) {
        float spriteX = enemy.x - posX;
        float spriteY = enemy.y - posY;

        float invDet = 1.0f / (planeX * dirY - dirX * planeY);
        float transformX = invDet * (dirY * spriteX - dirX * spriteY);
        float transformY = invDet * (-planeY * spriteX + planeX * spriteY);

        if (transformY > 0.1f) {
            int spriteScreenX = (int)((128 / 2) * (1 + transformX / transformY));
            int spriteHeight = abs((int)(128 / transformY));
            int drawStartY = -spriteHeight / 2 + 64;
            if (drawStartY < 0) drawStartY = 0;
            int drawEndY = spriteHeight / 2 + 64;
            if (drawEndY >= 128) drawEndY = 127;

            int spriteWidth = abs((int)(128 / transformY));
            int drawStartX = -spriteWidth / 2 + spriteScreenX;
            if (drawStartX < 0) drawStartX = 0;
            int drawEndX = spriteWidth / 2 + spriteScreenX;
            if (drawEndX >= 128) drawEndX = 127;

            for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
                if (transformY < z_buffer[stripe]) {
                    // Draw enemy as a simple box or X
                    app_display_draw_rect(stripe, drawStartY + (spriteHeight/4), 1, spriteHeight/2, 0);
                }
            }
        }
    }

    // 3. UI and Flash
    if (shoot_flash > 0) {
        app_display_draw_rect(40, 100, 48, 20, 0); // Muzzle flash
        shoot_flash--;
    } else {
        // Simple Crosshair
        app_display_draw_rect(62, 64, 4, 1, 0);
        app_display_draw_rect(64, 62, 1, 4, 0);
    }

    app_display_update(DISPLAY_UPDATE_NORMAL);
}

static void Doom_Shoot(void) {
    shoot_flash = 2;
    if (enemy.active) {
        float spriteX = enemy.x - posX;
        float spriteY = enemy.y - posY;
        float invDet = 1.0f / (planeX * dirY - dirX * planeY);
        float transformX = invDet * (dirY * spriteX - dirX * spriteY);
        float transformY = invDet * (-planeY * spriteX + planeX * spriteY);
        
        // If enemy is roughly in the center of the screen
        if (transformY > 0) {
            int spriteScreenX = (int)((128 / 2) * (1 + transformX / transformY));
            if (abs(spriteScreenX - 64) < 20) {
                enemy.active = false;
            }
        }
    }
}

static void Doom_Rotate(void) {
    float oldDirX = dirX;
    dirX = dirX * cosf(ROT_SPEED) - dirY * sinf(ROT_SPEED);
    dirY = oldDirX * sinf(ROT_SPEED) + dirY * cosf(ROT_SPEED);
    float oldPlaneX = planeX;
    planeX = planeX * cosf(ROT_SPEED) - planeY * sinf(ROT_SPEED);
    planeY = oldPlaneX * sinf(ROT_SPEED) + planeY * cosf(ROT_SPEED);
}

static void Doom_Update(void) {
    float nextX = posX + dirX * STEP_SIZE;
    float nextY = posY + dirY * STEP_SIZE;
    if (world_map[(int)nextX][(int)posY] == 0) posX = nextX;
    if (world_map[(int)posX][(int)nextY] == 0) posY = nextY;

    // Respawn enemy if killed
    if (!enemy.active && (rand() % 20 == 0)) {
        enemy.x = 2 + (rand() % (MAP_SIZE - 4));
        enemy.y = 2 + (rand() % (MAP_SIZE - 4));
        enemy.active = true;
    }
}

static void Doom_OnEnter(void) {
    posX = 1.5f; posY = 1.5f;
    dirX = 1.0f; dirY = 0.0f;
    planeX = 0.0f; planeY = 0.66f;
    enemy.active = true;
    Doom_Draw();
}

static void Doom_OnEvent(UI_Event_t event) {
    if (event == EVENT_TICK_100MS) {
        static uint8_t move_tick = 0;
        if (++move_tick >= 3) {
            move_tick = 0;
            Doom_Update();
            Doom_Draw();
        }
    } 
    else if (event == EVENT_BTN_NEXT) {
        Doom_Rotate();
        Doom_Draw();
    }
    else if (event == EVENT_BTN_DOUBLE) {
        Doom_Shoot();
        Doom_Draw();
    }
    else if (event == EVENT_BTN_SELECT) {
        UI_Manager_SwitchScreen(&Screen_Menu);
    }
}

const AppScreen_t Screen_Doom = {
    .name = "Doom",
    .on_enter = Doom_OnEnter,
    .on_event = Doom_OnEvent,
    .on_exit = NULL
};
