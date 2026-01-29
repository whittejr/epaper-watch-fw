/**
* @file    ls013b7dh03.c
* @brief   none
* @version 0.1.0
* @author  Alessandro Davi
* @date    2026-01-27
*/

// #include "shm_graphics.h"
#include "resources.h"
#include "shmfonts.h"
#include "LS013B7DH03.h"
#include "main.h"
#include "stm32wbxx_hal.h"
#include <string.h>

// Screen object
LCD_128x128_t shm_128x128;

// Screenbuffer
uint8_t LCD_128x128_Buffer[LCD_BUFFER_SIZE];

/*
*   FUNCTIONS
*/

uint8_t ls013b7dh03_init(ls013b7dh03_handle_t *handle) {
	
	/*
	*	verify
	*/
	if (handle->spi_init == NULL)
		return 1;

	if (handle->spi_deinit == NULL)
		return 1;

	if (handle->spi_write_cmd == NULL)
		return 1;

	if (handle->gpio_init == NULL)
		return 1;

	/*
	*	init
	*/
	if (handle->gpio_init() != 0)
		return 1;

	if (handle->spi_init() != 0)
		return 1;

    /*Configure Display Struct*/
	shm_128x128.CurrentX = 0;
	shm_128x128.CurrentY = 0;
	shm_128x128.Inverted = 0; // false?
	shm_128x128.Initialized = 1; // true?

	return 0;
}

uint8_t ls013b7dh03_clear(ls013b7dh03_handle_t *handle) {
    uint8_t res;
    uint8_t buf[2] = {SHARPMEM_BIT_CLEAR , 0x00};
    if (handle == NULL)
        return 1;

    res = handle->spi_write_cmd(buf, 2);
    if (res != 0)
        return 1;

    if(shm_128x128.Inverted == 0)
		memset((void*)LCD_128x128_Buffer, 0x00, LCD_BUFFER_SIZE);
    else 
	    memset((void*)LCD_128x128_Buffer, 0xFF, LCD_BUFFER_SIZE);
    
    shm_128x128.CurrentX = 0;
	shm_128x128.CurrentY = 0;

    return 0;
}

/**
  * @brief  Draw a single pixel on LCD.
  * @note   Send the Clear command to the display and initialise the LCD_128x128_Buffer.
  * @param  uint8_t x => X position .
  * @param  uint8_t y => Y position .
  * @param  LCD_COLOR color => Color of the pixel, Black or White .
  * @retval None.
  */
void ls013b7dh03_drawPixel(uint8_t x, uint8_t y, LCD_COLOR color){
	uint16_t buff_addr = x/8 + (LCD_WIDTH/8)*y ;
	uint8_t bit_to_set = 1 << ((x % 8));

    if(x >= LCD_WIDTH || y >= LCD_HEIGHT) {	// Don't write outside the buffer
        return;
    }
    // Check if pixel should be inverted
    if(shm_128x128.Inverted)
        color = (LCD_COLOR)!color;

    // Draw in the right color
    if(color == White)
    	LCD_128x128_Buffer[buff_addr] |= bit_to_set;
    else
    	LCD_128x128_Buffer[buff_addr] &= ~bit_to_set;
}

/**
  * @brief  Write single character on LCD.
  * @note   Automatic new line and checking of remaining space.
  * @param  char ch => Character to write .
  * @param  FontDef Font => Font to use .
  * @param  LCD_COLOR color => Color, Black or White .
  * @retval Return written char for validation.
  */
char ls013b7dh03_writeChar(char ch, FontDef Font, LCD_COLOR color){
    uint32_t i, b, j;

    // Check if character is valid
    if (ch < 32 || ch > 126)
        return 0;
    // Check remaining space on current line
    if(LCD_WIDTH < (shm_128x128.CurrentX + Font.FontWidth)) {
    	if(LCD_HEIGHT > (shm_128x128.CurrentY + 2*Font.FontHeight -2)){		// -2 => Margin of the character
    		shm_128x128.CurrentX = 0;
    		shm_128x128.CurrentY = shm_128x128.CurrentY + Font.FontHeight - 1;		//-1 => Margin of the character fort
    	} else {
    		return 0;
    	}
    }

    // Use the font to write
    for(i = 0; i < Font.FontHeight; i++) {
        b = Font.data[(ch - 32) * Font.FontHeight + i];
        for(j = 0; j < Font.FontWidth; j++) {
            if((b << j) & 0x8000)  {
            	ls013b7dh03_drawPixel(shm_128x128.CurrentX + j, (shm_128x128.CurrentY + i), (LCD_COLOR) color);
            } else {
            	ls013b7dh03_drawPixel(shm_128x128.CurrentX + j, (shm_128x128.CurrentY + i), (LCD_COLOR)!color);
            }
        }
    }
    // The current space is now taken
    shm_128x128.CurrentX += Font.FontWidth;

    // Return written char for validation
    return ch;
}


/**
  * @brief  LCD refresh - send the complete LCD_128x128_Buffer to internal memory display.
  * @note   Blocking function.
  * @param  None.
  * @retval None.
  */
uint8_t ls013b7dh03_refresh(ls013b7dh03_handle_t *handle){
	uint8_t linebuf[18];
    uint8_t cmd = SHARPMEM_BIT_WRITECMD;
    uint8_t dummy = 0x00;

    handle->cs_control(1);

    handle->spi_write_refresh(&cmd, 1);

    for (uint8_t line = 1; line <= 128; line++) {
        linebuf[0] = line;

        memcpy(&linebuf[1],
               &LCD_128x128_Buffer[16 * (line - 1)],
               16);

        linebuf[17] = 0x00; // EOL obrigatório

        handle->spi_write_refresh(linebuf, 18);
    }

    handle->spi_write_refresh(&dummy, 1); // EOF

    handle->cs_control(0);

    return 0;
}

void ls013b7dh03_test(ls013b7dh03_handle_t *handle)
{
    if (handle == NULL)
        return;

    /* 1. Init do driver */
    if (ls013b7dh03_init(handle) != 0)
        return;

    HAL_Delay(1000);
    /* 2. Limpa a tela */
    // if (ls013b7dh03_clear(handle) != 0)
    //     return;

    /* 3. Teste de pixels (diagonal) */
    // for (uint8_t i = 0; i < 64; i++) {
    //     ls013b7dh03_drawPixel(i, i, Black);
    // }

    /* 4. Teste de texto */
    // shm_128x128.CurrentX = 0;
    // shm_128x128.CurrentY = 20;

    // ls013b7dh03_writeChar('O', Font_7x10, Black);
    // ls013b7dh03_writeChar('K', Font_7x10, Black);
    memset((void*)LCD_128x128_Buffer, 0x00, LCD_BUFFER_SIZE);
    /* 5. Refresh */
    
    ls013b7dh03_refresh(handle);
}