/*
 * hal_keypad.h
 *
 * Created: 7/6/2022 12:27:38 PM
 *  Author: Ramy
 */ 


#ifndef HAL_KEYPAD_H_
#define HAL_KEYPAD_H_


/* Section : Includes */

#include "../../MCAL/mcal_gpio/mcal_gpio.h"

/* Section: Macro Declarations */

#define KEYPAD_rows_count 4
#define KEYPAD_columns_count 4


/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */

typedef struct  
{
	pin_config_t keypad_row_pins[KEYPAD_rows_count];
	pin_config_t keypad_column_pins[KEYPAD_columns_count];

	
}keypad_t;

/* Section: Function Declarations */

Std_ReturnType keypad_intialize(const keypad_t *_keypad);
Std_ReturnType keypad_read_value(const keypad_t *_keypad, uint8 *_value );

#endif /* HAL_KEYPAD_H_ */