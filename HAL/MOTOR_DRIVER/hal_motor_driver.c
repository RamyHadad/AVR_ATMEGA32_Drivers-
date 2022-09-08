/*
 * hal_motor_driver.c
 *
 * Created: 7/3/2022 3:43:52 AM
 *  Author: Ramy
 */ 

#include "hal_motor_driver.h"


Std_ReturnType dc_motor_intialize(const dc_motor_t * dc_motor){
	Std_ReturnType ret = E_OK;
	if(NULL == dc_motor){
		ret = E_NOT_OK;
	}
	else{
		
		 gpio_pin_direction_intialize(&(dc_motor->motor_pins[motor_pin_1]));
		 gpio_pin_direction_intialize(&(dc_motor->motor_pins[motor_pin_2]));
		 gpio_pin_write_logic(&(dc_motor->motor_pins[motor_pin_1]),	GPIO_LOW );
		 gpio_pin_write_logic(&(dc_motor->motor_pins[motor_pin_2]),	GPIO_LOW );
		  
		  
		}
	
	return ret;
}

Std_ReturnType dc_motor_turn_right(const dc_motor_t * dc_motor){
	Std_ReturnType ret = E_OK;
	if(NULL == dc_motor){
		ret = E_NOT_OK;
	}
	else{
		 gpio_pin_write_logic(&(dc_motor->motor_pins[motor_pin_1]),	GPIO_HIGH );
		 gpio_pin_write_logic(&(dc_motor->motor_pins[motor_pin_2]),	GPIO_LOW );
	}

return ret;
}


Std_ReturnType dc_motor_turn_left(const dc_motor_t * dc_motor){
	
	Std_ReturnType ret = E_OK;
	
	if(NULL == dc_motor)
	{
		ret = E_NOT_OK;
		}
	else
	{
		 gpio_pin_write_logic(&(dc_motor->motor_pins[motor_pin_1]),	GPIO_LOW );
		 gpio_pin_write_logic(&(dc_motor->motor_pins[motor_pin_2]),	GPIO_HIGH );
	}
return ret;
}


Std_ReturnType dc_motor_stop(const dc_motor_t * dc_motor){
	Std_ReturnType ret = E_OK;
	if(NULL == dc_motor){
		ret = E_NOT_OK;
	}
	else{
		
		 gpio_pin_write_logic(&(dc_motor->motor_pins[motor_pin_1]),	GPIO_LOW );
		 gpio_pin_write_logic(&(dc_motor->motor_pins[motor_pin_2]),	GPIO_LOW );
	}

return ret;
}

