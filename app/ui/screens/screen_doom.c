/**
* @file    screen_doom.c
* @brief   Upgraded Doom Lite with Weapon Sprite, Scaled Enemies, and Improved UI
*/

#include "ui_manager.h"
#include "app_display.h"
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define MAP_SIZE 8
#define FOV 0.66f
#define STEP_SIZE 0.12f
#define ROT_SPEED (3.14159f / 4.0f) // 45 degrees

extern const AppScreen_t Screen_Menu;

// Bitmaps
static const uint8_t doom_gun[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x07, 0xe0, 0x00,
    0x00, 0x0f, 0xf0, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x1f, 0xf8, 0x00,
    0x00, 0x1f, 0xf8, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x3f, 0xfc, 0x00,
    0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x7f, 0xfe, 0x00,
    0x00, 0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xc0,
    0x07, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xfc,
    0x7f, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

static const uint8_t doom_enemy[] = {
    0x07, 0xe0, 0x1f, 0xf8, 0x3f, 0xfc, 0x7f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xcf, 0xf0, 0x0f,
    0xf0, 0x0f, 0xf3, 0xcf, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xfe, 0x3f, 0xfc, 0x1f, 0xf8, 0x07, 0xe0
};

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

#define MAX_ENEMIES 3
static Sprite_t enemies[MAX_ENEMIES];
static float posX = 1.5f, posY = 1.5f;
static float dirX = 1.0f, dirY = 0.0f;
static float planeX = 0.0f, planeY = 0.66f;
static float z_buffer[128];
static uint8_t shoot_anim = 0;
static uint16_t kill_count = 0;

static void Doom_Draw(void) {
    app_display_clear();

    // 1. Sky & Floor
    app_display_draw_rect(0, 64, 128, 1, 0); // Horizon line

    // 2. Raycasting Walls
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

    // 3. Sprite Rendering (Enemies)
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;

        float spriteX = enemies[i].x - posX;
        float spriteY = enemies[i].y - posY;

        float invDet = 1.0f / (planeX * dirY - dirX * planeY);
        float transformX = invDet * (dirY * spriteX - dirX * spriteY);
        float transformY = invDet * (-planeY * spriteX + planeX * spriteY);

        if (transformY > 0.1f) {
            int spriteScreenX = (int)((128 / 2) * (1 + transformX / transformY));
            int spriteHeight = abs((int)(128 / transformY));
            if (spriteHeight > 100) spriteHeight = 100;

            int drawStartY = -spriteHeight / 2 + 64;
            int drawEndY = spriteHeight / 2 + 64;

            int spriteWidth = spriteHeight; // Square sprites
            int drawStartX = -spriteWidth / 2 + spriteScreenX;
            int drawEndX = spriteWidth / 2 + spriteScreenX;

            for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
                if (stripe >= 0 && stripe < 128 && transformY < z_buffer[stripe]) {
                    // Simple scaled pixel drawing for the enemy bitmap
                    int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * 16 / spriteWidth) / 256;
                    for (int y = drawStartY; y < drawEndY; y++) {
                        if (y >= 0 && y < 128) {
                            int texY = (int)(256 * (y - (-spriteHeight / 2 + 64)) * 16 / spriteHeight) / 256;
                            uint8_t pixel = (doom_enemy[texY * 2 + (texX / 8)] >> (7 - (texX % 8))) & 0x01;
                            if (pixel) {
                                app_display_draw_rect(stripe, y, 1, 1, 0);
                            }
                        }
                    }
                }
            }
        }
    }

    // 4. Weapon
    int gun_y_offset = (shoot_anim > 0) ? 10 : 0;
    app_display_draw_bitmap(48, 96 + gun_y_offset, doom_gun, 32, 32, 0);
    if (shoot_anim > 0) {
        app_display_draw_rect(60, 85, 8, 8, 0); // Muzzle flash
        shoot_anim--;
    }

    // 5. HUD
    char hud[16];
    snprintf(hud, sizeof(hud), "KILLS: %d", kill_count);
    app_display_draw_text(2, 2, hud, 0);
    
    app_display_update(DISPLAY_UPDATE_NORMAL);
}

static void Doom_Shoot(void) {
    shoot_anim = 2;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;

        float spriteX = enemies[i].x - posX;
        float spriteY = enemies[i].y - posY;
        float invDet = 1.0f / (planeX * dirY - dirX * planeY);
        float transformX = invDet * (dirY * spriteX - dirX * spriteY);
        float transformY = invDet * (-planeY * spriteX + planeX * spriteY);
        
        if (transformY > 0) {
            int spriteScreenX = (int)((128 / 2) * (1 + transformX / transformY));
            if (abs(spriteScreenX - 64) < 15) {
                enemies[i].active = false;
                kill_count++;
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

    // Respawn enemies
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active && (rand() % 10 == 0)) {
            enemies[i].x = 2 + (rand() % (MAP_SIZE - 4)) + 0.5f;
            enemies[i].y = 2 + (rand() % (MAP_SIZE - 4)) + 0.5f;
            enemies[i].active = true;
        }
    }
}

static void Doom_OnEnter(void) {
    posX = 1.5f; posY = 1.5f;
    dirX = 1.0f; dirY = 0.0f;
    planeX = 0.0f; planeY = 0.66f;
    kill_count = 0;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = (i == 0);
        enemies[i].x = 4.5f; enemies[i].y = 4.5f;
    }
    Doom_Draw();
}

static void Doom_OnEvent(UI_Event_t event) {
    if (event == EVENT_TICK_100MS) {
        static uint8_t move_tick = 0;
        if (++move_tick >= 2) {
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
