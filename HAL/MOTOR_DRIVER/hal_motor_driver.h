/*
 * hal_motor_driver.h
 *
 * Created: 7/3/2022 3:43:39 AM
 *  Author: Ramy
 */ 


#ifndef HAL_MOTOR_DRIVER_H_
#define HAL_MOTOR_DRIVER_H_

/* Section : Includes */
#include "../../MCAL/mcal_gpio/mcal_gpio.h"

/* Section: Macro Declarations */
#define motor_status_on   1
#define motor_status_off  0

#define motor_pin_1  0
#define motor_pin_2  1


/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */

typedef struct {
	
	pin_config_t motor_pins[2];
	
}dc_motor_t;

/* Section: Function Declarations */

Std_ReturnType dc_motor_intialize(const dc_motor_t * dc_motor);
Std_ReturnType dc_motor_turn_right(const dc_motor_t * dc_motor);
Std_ReturnType dc_motor_turn_left(const dc_motor_t * dc_motor);
Std_ReturnType dc_motor_stop(const dc_motor_t * dc_motor);

#endif /* HAL_MOTOR_DRIVER_H_ */