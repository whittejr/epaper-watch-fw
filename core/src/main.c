/**
* @file     main.c
* @brief    none
* @version  0.1.0
* @author   Alessandro Davi
* @date     2026-03-31
*/

#include "app_display.h"
#include "app_system.h"



int main(void) {
    app_system_init();

    app_display_draw_text();
    app_display_update();

    app_system_loop();
    return 0;
}
