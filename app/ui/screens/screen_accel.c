/**
* @file    screen_accel.c
* @brief   Accelerometer data visualizer with real-time bars
*/

#include "ui_manager.h"
#include "app_display.h"
#include "app_accel.h"
#include <stdio.h>
#include <math.h>

extern const AppScreen_t Screen_Menu;

static void draw_axis_bar(int y, const char* label, float val) {
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%s:%+.2f", label, val);
    app_display_draw_text(2, y, buffer, 0);

    // O texto ocupa aprox 49 pixels. Começar a barra em x=52.
    int bar_x = 52;
    int bar_w = 72; // Largura total para caber até x=124
    int center_x = bar_x + (bar_w / 2);
    
    // Desenha uma moldura VAZIA (usando 4 linhas)
    // O gfx_draw_rect na verdade preenche o retângulo inteiro, 
    // então se desenhássemos um retângulo grande, ele seria um bloco preto!
    app_display_draw_rect(bar_x, y + 2, bar_w, 1, 0);              // Topo
    app_display_draw_rect(bar_x, y + 10, bar_w, 1, 0);             // Fundo
    app_display_draw_rect(bar_x, y + 2, 1, 9, 0);                  // Esquerda
    app_display_draw_rect(bar_x + bar_w - 1, y + 2, 1, 9, 0);      // Direita
    
    // Linha central (0G) maior
    app_display_draw_rect(center_x, y + 1, 1, 11, 0);
    
    // Sensibilidade do gráfico: 15 pixels por G
    int fill_w = (int)(val * 15.0f); 
    if (fill_w > 34) fill_w = 34;
    if (fill_w < -34) fill_w = -34;

    // Preenchimento interno (usando a capacidade de fill do draw_rect)
    if (fill_w > 0) {
        app_display_draw_rect(center_x, y + 4, fill_w, 5, 0);
    } else if (fill_w < 0) {
        app_display_draw_rect(center_x + fill_w, y + 4, -fill_w, 5, 0);
    }
}

static void Accel_Draw(display_update_mode_t mode) {
    char buffer[32];
    float g[3];
    
    app_display_clear();
    app_display_draw_text_aligned(64, 5, 1, "SENSOR MOVIMENTO", 0);
    app_display_draw_rect(10, 16, 108, 1, 0);

    // Lê os valores em cache do sensor sem acessar o SPI diretamente
    app_accel_get_last_g(g);
    
    // Desenha barras para X, Y, Z
    draw_axis_bar(30, "X", g[0]);
    draw_axis_bar(50, "Y", g[1]);
    draw_axis_bar(70, "Z", g[2]);

    // Rodapé com passos
    app_display_draw_rect(0, 95, 128, 1, 0);
    snprintf(buffer, sizeof(buffer), "PASSOS: %lu", app_accel_get_steps());
    app_display_draw_text_aligned(64, 105, 1, buffer, 0);

    app_display_update(mode);
}

static void Accel_OnEnter(void) {
    Accel_Draw(DISPLAY_UPDATE_NORMAL);
}

static void Accel_OnEvent(UI_Event_t event) {
    // Atualiza a cada 100ms para parecer tempo real
    if (event == EVENT_TICK_100MS) {
        Accel_Draw(DISPLAY_UPDATE_PARTIAL);
    } else if (event == EVENT_BTN_SELECT) {
        UI_Manager_SwitchScreen(&Screen_Menu);
    }
}

const AppScreen_t Screen_Accel = {
    .name = "Accel",
    .on_enter = Accel_OnEnter,
    .on_event = Accel_OnEvent,
    .on_exit = NULL
};
