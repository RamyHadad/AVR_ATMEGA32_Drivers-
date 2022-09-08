/*
 * hal_lcd_driver.h
 *
 * Created: 7/7/2022 12:44:24 PM
 *  Author: Ramy
 */ 


#ifndef HAL_LCD_DRIVER_H_
#define HAL_LCD_DRIVER_H_

/* Section : Includes */
#include <stdlib.h>
#include <string.h>
#include "../../MCAL/mcal_gpio/mcal_gpio.h"
#define F_CPU  Processor_clock
#include <util/delay.h>


/* Section: Macro Declarations */

#define	_lcd_clear_display			   0x01
#define	_lcd_return_home               0x02
#define _lcd_entry_mode                0x06
#define _lcd_cursor_off_display_on     0x0C
#define _lcd_cursor_off_display_off    0x08
#define _lcd_cursor_on_blink_on        0x0F
#define _lcd_cursor_on_blink_off       0X0E
#define _lcd_display_shift_right       0x1C
#define _lcd_display_shift_left        0x18

#define _lcd_8bit_mode_2_line          0x38 // function set 
#define _lcd_4bit_mode_2_line          0x28 // function set 

#define _lcd_CGRAM_start               0x40
#define _lcd_DDRAM_start               0x80

#define _lcd_DDRAM_row1                0x80
#define _lcd_DDRAM_row2                0xc0
#define _lcd_DDRAM_row3                0x90
#define _lcd_DDRAM_row4                0xd0


#define _lcd_4bit                        4
#define _lcd_8bit                        8

// LCD bit mode config 
#define _lcd_mode_bit_set            _lcd_4bit  


/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */

typedef struct{
	
	pin_config_t lcd_enable;
	pin_config_t lcd_rs;
    pin_config_t lcd_data[_lcd_mode_bit_set];
	
}lcd_t;

/* Section: Function Declarations */
Std_ReturnType lcd_intialize(const lcd_t *_lcd);
Std_ReturnType lcd_send_command(const lcd_t *_lcd, uint8 command);
Std_ReturnType lcd_send_char_data(const lcd_t *_lcd, uint8 data);
Std_ReturnType lcd_send_char_data_pos(const lcd_t *_lcd, uint8  row , uint8 coloumn , uint8 data);
Std_ReturnType lcd_send_string_data(const lcd_t *_lcd, uint8 *str);
Std_ReturnType lcd_send_string_data_pos(const lcd_t *_lcd, uint8  row , uint8 coloumn , uint8 *str);

Std_ReturnType lcd_send_uint16_data_pos(const lcd_t *_lcd, uint8  row , uint8 coloumn , uint16 int_value);
Std_ReturnType lcd_send_uint8_data_pos(const lcd_t *_lcd, uint8  row , uint8 coloumn , uint8 int_value);
Std_ReturnType lcd_send_uint32_data_pos(const lcd_t *_lcd, uint8  row , uint8 coloumn , uint16 int_value);

Std_ReturnType convert_uint8_to_string(uint8 value, uint8 *str);
Std_ReturnType convert_uint16_to_string(uint16 value, uint8 *str);
Std_ReturnType convert_uint32_to_string(uint32 value, uint8 *str);

#endif /* HAL_LCD_DRIVER_H_ */