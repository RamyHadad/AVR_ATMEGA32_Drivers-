/*
 * hal_button.h
 *
 * Created: 7/1/2022 7:15:18 AM
 *  Author: Ramy
 */ 


#ifndef HAL_BUTTON_H_
#define HAL_BUTTON_H_


/* ----------------- Includes -----------------*/
#include "../../MCAL/mcal_gpio/mcal_gpio.h"


/* ----------------- Macro Declarations -----------------*/

/* ----------------- Macro Functions Declarations -----------------*/

/* ----------------- Data Type Declarations -----------------*/
typedef enum{
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED
}button_state_t;

typedef enum{
    BUTTON_ACTIVE_LOW,
    BUTTON_ACTIVE_HIGH
}button_active_t;

typedef struct{
    pin_config_t button_pin;
    button_state_t button_state;
    button_active_t button_connection;
}button_t;

/* ----------------- Software Interfaces Declarations -----------------*/
/**
 * @brief Initialize the assigned pin to be Input.
 * @param btn pointer to the button configurations
 * @return  Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue while performing this action
 */
Std_ReturnType button_initialize(const button_t *btn);

/**
 * @brief Read the state of the button
 * @param btn pointer to the button configurations
 * @param btn_state button state @ref button_state_t
 * @return  Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue while performing this action
 */
Std_ReturnType button_read_state(const button_t *btn, button_state_t *btn_state);


#endif /* HAL_BUTTON_H_ */