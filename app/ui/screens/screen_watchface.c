#include "ui_manager.h"
#include "app_display.h"
#include <stdio.h>

extern const AppScreen_t Screen_Menu; // Declara a existência da outra tela

static uint32_t segundos = 0;

static void Watchface_Draw(display_update_mode_t mode) {
    char buffer[32];
    
    app_display_clear(); // Limpa o buffer local (RAM preta)
    
    // Desenha o título e as instruções
    app_display_draw_text(20, 20, "WATCHFACE");
    app_display_draw_text(10, 160, "Bem Vindo, Alessandro");

    // Desenha o dado dinâmico
    // snprintf(buffer, sizeof(buffer), "Tempo: %u", segundos);
    // app_display_draw_text(20, 80, buffer);
    
    // Dispara a atualização física
    app_display_update(mode);
}

static void Watchface_OnEnter(void) {
    // Ao entrar na tela, sempre fazemos um FULL REFRESH
    Watchface_Draw(SSD1681_UPDATE_FAST);
}

static void Watchface_OnEvent(UI_Event_t event) {
    if (event == EVENT_TICK_1SEC) {
        segundos++;
        // Atualiza apenas os números, sem piscar (PARTIAL)
        Watchface_Draw(SSD1681_UPDATE_PARTIAL); 
    }
    else if (event == EVENT_BTN_SELECT) {
        // Transição de tela!
        UI_Manager_SwitchScreen(&Screen_Menu);
    }
}

const AppScreen_t Screen_Watchface = {
    .name = "Watchface",
    .on_enter = Watchface_OnEnter,
    .on_event = Watchface_OnEvent,
    .on_exit = NULL // Não precisamos fazer nada ao sair
};