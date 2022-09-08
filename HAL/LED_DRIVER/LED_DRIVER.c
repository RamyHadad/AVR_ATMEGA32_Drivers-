/*
 * LED_DRIVER.c
 *
 * Created: 7/1/2022 6:07:00 AM
 *  Author: Ramy
 */ 

/* Section : Includes */

#include "LED_DRIVER.h"

/* Section: Function Declarations */
Std_ReturnType led_initialize(const led_t *led){
	Std_ReturnType ret = E_OK ; 
	if (NULL == led)
	{
		ret = E_NOT_OK ;
	} 
	else
	{  pin_config_t LED_setup = {.direction = GPIO_DIRECTION_OUTPUT ,
		 .port = led ->port_name ,
		 .pin = led ->pin ,
		 .logic = led ->led_status};
		gpio_pin_intialize(&LED_setup);
	}
	
	return ret;
	
}

Std_ReturnType led_turn_on(const led_t *led){
	Std_ReturnType ret = E_OK ;
	if (NULL == led)
	{
		ret = E_NOT_OK ;
		
	}
	else
	{   pin_config_t LED_setup = {.direction = GPIO_DIRECTION_OUTPUT ,
		.port = led ->port_name ,
		.pin = led ->pin ,
	    .logic = led ->led_status};
	     gpio_pin_write_logic(&LED_setup, GPIO_HIGH);
		
	}
	
	return ret;
	
	
}
Std_ReturnType led_turn_off(const led_t *led){
	Std_ReturnType ret = E_OK ;
	if (NULL == led)
	{
		ret = E_NOT_OK ;
		
	}
	else
	{  pin_config_t LED_setup = {.direction = GPIO_DIRECTION_OUTPUT ,
		.port = led ->port_name ,
		.pin = led ->pin ,
	.logic = led ->led_status};
	gpio_pin_write_logic(&LED_setup, GPIO_LOW);
		
	}
	
	return ret;
	
}
Std_ReturnType led_turn_toggle(const led_t *led){
	Std_ReturnType ret = E_OK ;
	if (NULL == led)
	{
		ret = E_NOT_OK ;
		
	}
	else
	{
		 pin_config_t LED_setup = {.direction = GPIO_DIRECTION_OUTPUT ,
			 .port = led ->port_name ,
			 .pin = led ->pin ,
		     .logic = led ->led_status};
			 
			gpio_pin_toggle_logic(&LED_setup);
	}
	
	return ret;
	
}
