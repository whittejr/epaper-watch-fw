/*
 * epaper_lib.c
 *
 *  Created on: Jul 7, 2025
 *      Author: alessandro.davi
 */


#include "ssd1681.h"
#include "bmp.h"
#include <stdio.h>
#include <string.h>

#define EPD_154

#define SECTION_CLOCK_Y  10
#define SECTION_FACE_Y   70
#define SECTION_TEMP_Y   150

#define FONT_WIDTH 12   // largura em pixels da fonte 24x12
#define FONT_HEIGHT 24  // altura da fonte 24x12

extern epd_config_t epd;

int main(void) {

	epd_init();

	// --- Inicializa área de pintura ---
	epd_paint_newimage(epd.framebuffer, epd.width, epd.height, EPD_ROTATE_90, EPD_COLOR_WHITE);
	epd_paint_clear(EPD_COLOR_WHITE);

	// --- Textos e cálculos de centralização ---
	char time_str[] = "20:13";
	char temp_str[] = "Temp: 25C";

	uint16_t center_x_time = (epd.width - FONT_WIDTH * strlen(time_str)) / 2;
	uint16_t center_x_face = (epd.width - FONT_WIDTH * 2) / 2;
	uint16_t center_x_temp = (epd.width - FONT_WIDTH * strlen(temp_str)) / 2;

	// --- Desenho ---
//	epd_paint_showString(&epd, center_x_time, SECTION_CLOCK_Y, (uint8_t *)time_str, EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);
//
//	epd_paint_showString(&epd, center_x_face, SECTION_FACE_Y, (uint8_t *)":)", EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);
//
//	epd_paint_showString(&epd, center_x_temp, SECTION_TEMP_Y, (uint8_t *)temp_str, EPD_FONT_SIZE24x12, EPD_COLOR_BLACK);
//

	// --- Envia framebuffer para o display ---
//	epd_displayBW(&epd, epd.framebuffer);

	char buffer[32];
	uint16_t center_x_str = (epd.width - FONT_WIDTH * strlen(buffer)) / 2;
//	epd_paint_showString(&epd, 0, SECTION_CLOCK_Y,  (uint8_t *) buffer, EPD_FONT_SIZE8x6, EPD_COLOR_BLACK);
//	epd_displayBW(&epd, epd.framebuffer);

	uint8_t cont = 0;
	// --- Dormência (deep sleep) ---
//	epd_enter_deepsleepmode(&epd, EPD_DEEPSLEEP_MODE1);
//
	for (int i = 0; i < 20; i++) {
		sprintf(buffer, "Pedro eh gay? SIM: %d | NAO: 0", cont);
		epd_paint_showString(0, SECTION_FACE_Y,  (uint8_t *) buffer, EPD_FONT_SIZE12x6, EPD_COLOR_BLACK);
		epd_displayBW_partial(epd.framebuffer);
		cont += 1;
		delay_100ms();
	}
	// --- Teste de wake/reset + nova exibição (pode remover depois) ---
//	epd_reset(&epd);
//	delay_1000ms();
//	epd_displayBW(&epd, epd.framebuffer);

	return 0;
}