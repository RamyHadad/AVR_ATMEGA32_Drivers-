/*
 * hal_keypad.c
 *
 * Created: 7/6/2022 12:27:53 PM
 *  Author: Ramy
 */ 

/* Section : Includes */

#include "hal_keypad.h"

/* Section: Data Type Declarations */

const static char keypad_arr[KEYPAD_rows_count][KEYPAD_columns_count] = {            
                                        
																{'7','8','9','/'},
																{'4','5','6','x'},
																{'1','2','3','-'},
																{'#','0','=','+'}
										
};

/* Section: Function Definition */

 Std_ReturnType keypad_intialize(const keypad_t *_keypad){
	Std_ReturnType ret = E_OK;  
	if(NULL == _keypad ){
		ret = E_NOT_OK;
	}
	else{
		
		for (int row_index = 0; row_index<KEYPAD_rows_count; row_index++){
			
	          ret = gpio_pin_direction_intialize(&(_keypad->keypad_row_pins[row_index]));
	    	  ret = gpio_pin_write_logic((&(_keypad->keypad_row_pins[row_index])), GPIO_LOW);
		   
		}
		
		for (int column_index = 0; column_index<KEYPAD_columns_count; column_index++){
		
		 ret = gpio_pin_direction_intialize(&(_keypad->keypad_column_pins[column_index]));
		 
		}
	}
	return ret;
}

Std_ReturnType keypad_read_value(const keypad_t *_keypad, uint8 *_value ){
	Std_ReturnType ret = E_OK;  
	if(NULL == _keypad ){
		ret = E_NOT_OK;
	}
	else{
		
		for (int row_index = 0; row_index<KEYPAD_rows_count; row_index++)
		{	
			for (int column_index = 0; column_index<KEYPAD_columns_count; column_index++)
			
			{  ret = gpio_pin_write_logic(&(_keypad->keypad_row_pins[row_index]),  GPIO_HIGH);

				logic_t read= 0;
				
				ret = gpio_pin_read_logic(&(_keypad->keypad_column_pins[column_index]), & read );
				if (read == GPIO_HIGH )
				{
					*_value = keypad_arr[row_index][column_index] ;
				}
				ret = gpio_pin_write_logic(&(_keypad->keypad_row_pins[row_index]),  GPIO_LOW);
			}
			
		}
		
	}
 return ret;
}


