/*
 * LED_DRIVER.h
 *
 * Created: 7/1/2022 6:06:51 AM
 *  Author: Ramy
 */ 


#ifndef LED_DRIVER_H_
#define LED_DRIVER_H_


/* Section : Includes */
#include "../../MCAL/mcal_gpio/mcal_gpio.h"

/* Section: Macro Declarations */

/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */
typedef enum{
	LED_OFF,
	LED_ON
}led_status_t;

typedef struct{
	port_index_t port_name : 4;
	pin_index_t pin : 3;
	led_status_t led_status : 1;
}led_t;

/* Section: Function Declarations */
Std_ReturnType led_initialize(const led_t *led);
Std_ReturnType led_turn_on(const led_t *led);
Std_ReturnType led_turn_off(const led_t *led);
Std_ReturnType led_turn_toggle(const led_t *led);



#endif /* LED_DRIVER_H_ */