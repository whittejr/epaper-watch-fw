#include "ssd1681.h"
#include "ui_manager.h"
#include "app_display.h"

// Avisa o compilador que a tela de configuração existe em outro arquivo
extern const AppScreen_t Screen_Config; 

static const char *menu_items[] = {
    "Oximetro",
    "Acelerometro",
    "Ver Dados",
    "Ajustar Hora",
    "Configuracoes",
    "Voltar"
};
#define MENU_COUNT 6

static uint8_t cursor_menu = 0;

static void Menu_Draw(display_update_mode_t mode) {
    app_display_clear(); // Limpa a RAM do MCU (não a tela física)
    
    app_display_draw_text(60, 5, "MENU"); // Título

    // Desenha a lista de opções
    for (int i = 0; i < MENU_COUNT; i++) {
        uint16_t pos_y = 40 + (i * 26); // Começa no Y=40 e desce 26px por item
        
        if (i == cursor_menu) {
            app_display_draw_text(5, pos_y, ">"); // Desenha o cursor
        }
        app_display_draw_text(25, pos_y, menu_items[i]); // Desenha o texto
    }
    
    app_display_update(mode);
}

static void Menu_OnEnter(void) {
    cursor_menu = 0; // Sempre começa no topo
    Menu_Draw(SSD1681_UPDATE_FULL); // Limpa os fantasmas da tela anterior
}

static void Menu_OnEvent(UI_Event_t event) {
    if (event == EVENT_BTN_NEXT) {
        cursor_menu++;
        if (cursor_menu >= MENU_COUNT) cursor_menu = 0; // Volta pro topo se passar do limite
        
        Menu_Draw(SSD1681_UPDATE_PARTIAL); // Atualiza só o cursor movendo!
    }
    else if (event == EVENT_BTN_SELECT) {
        // Verifica em qual opção o cursor está quando o Select foi apertado
        // if (cursor_menu == 4) { 
            // "Configuracoes" é o índice 4
            UI_Manager_SwitchScreen(&Screen_Config);
        // }
        // Aqui você adicionaria os ifs para o Oximetro (0), etc...
    }
}

const AppScreen_t Screen_Menu = {
    .name = "Menu Principal",
    .on_enter = Menu_OnEnter,
    .on_event = Menu_OnEvent,
    .on_exit = NULL
};