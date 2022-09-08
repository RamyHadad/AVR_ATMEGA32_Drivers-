/*
 * hal_button.c
 *
 * Created: 7/1/2022 7:15:04 AM
 *  Author: Ramy
 */ 
/**
 * @brief Initialize the direction of a specific pin @ref direction_t
 * @param _pin_config pointer to the configurations @ref pin_config_t 
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */

#include "hal_button.h"

Std_ReturnType button_initialize(const button_t *btn){
	 Std_ReturnType ret = E_OK;
	 if(NULL == btn ){
		 ret = E_NOT_OK;
	 }
	 
	 else{ 
		 
		 ret = gpio_pin_direction_intialize( &(btn-> button_pin )) ; 
		 
	 }
	
	 return ret;
}

Std_ReturnType button_read_state(const button_t *btn, button_state_t *btn_state){
	
	 Std_ReturnType ret = E_OK;
	 logic_t pin_button_status = GPIO_LOW;
	 
	 if((NULL == btn) || (NULL == btn_state))
	 {
		 ret = E_NOT_OK;
	 }
	else{
		
		gpio_pin_read_logic(&(btn->button_pin), &pin_button_status);
		if (btn->button_connection == BUTTON_ACTIVE_HIGH)
			{ if (pin_button_status == GPIO_HIGH)
					{
				 *btn_state = BUTTON_PRESSED ; 
					} 
			else
					{
				*btn_state = BUTTON_RELEASED ; 
					}
			
			} 
		else if (btn->button_connection == BUTTON_ACTIVE_LOW)
		{
			if (pin_button_status == GPIO_HIGH)
					{
				*btn_state = BUTTON_RELEASED ;
					}
			else
					{
				*btn_state = BUTTON_PRESSED ;
					}
		
		}
	}
return ret;
	
}