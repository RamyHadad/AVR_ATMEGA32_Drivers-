/*
 * mcal_gpio.h
 *
 * Created: 6/26/2022 12:51:20 AM
 *  Author: Ramy
 */ 


#ifndef MCAL_GPIO_H_
#define MCAL_GPIO_H_

/* Section : Includes */

#include "../../LIB/std_types.h"
#include "mcal_gpio_cfg.h"
#include "../mcal_register_map.h"
#include  <stddef.h>

/* Section: Macro Declarations */
//#define NULL 0

#define BIT_MASK  (uint8)1
#define PORT_PIN_MAX_NUMBER  8
#define PORT_MAX_NUMBER      5
#define PORTC_MASK           0xFF


#define  PORTA_Reg  (( volatile unsigned char *) 0x3B)
#define  DDRA_Reg	(( volatile unsigned char *) 0x3A)
#define  PINA_Reg	(( volatile unsigned char *) 0x39)

#define  PORTB_Reg	(( volatile unsigned char *) 0x38)
#define  DDRB_Reg	(( volatile unsigned char *) 0x37)
#define  PINB_Reg	(( volatile unsigned char *) 0x36)

#define  PORTC_Reg	(( volatile unsigned char *) 0x35)
#define  DDRC_Reg	(( volatile unsigned char *) 0x34)
#define  PINC_Reg	(( volatile unsigned char *) 0x33)

#define  PORTD_Reg	(( volatile unsigned char *) 0x32)
#define  DDRD_Reg	(( volatile unsigned char *) 0x31)
#define  PIND_Reg	(( volatile unsigned char *) 0x30)

/* Section: Macro Functions Declarations */

#define HWREG8(_X)     (*((volatile uint8 *)(_X)))

#define SET_BIT(REG, BIT_POSN)     (REG |= (BIT_MASK << BIT_POSN))
#define CLEAR_BIT(REG, BIT_POSN)   (REG &= ~(BIT_MASK << BIT_POSN))
#define TOGGLE_BIT(REG, BIT_POSN)  (REG ^= (BIT_MASK << BIT_POSN))
#define READ_BIT(REG, BIT_POSN)    ((REG >> BIT_POSN) & BIT_MASK)

typedef void (*InterruptHandler)(void);


/* Section: Data Type Declarations */

typedef enum{
	GPIO_LOW = 0,
	GPIO_HIGH
}logic_t;

typedef enum{
	GPIO_DIRECTION_INPUT = 0,
	GPIO_DIRECTION_OUTPUT
}direction_t;

typedef enum{
	GPIO_PIN0 = 0,
	GPIO_PIN1,
	GPIO_PIN2,
	GPIO_PIN3,
	GPIO_PIN4,	
	GPIO_PIN5,
	GPIO_PIN6,
	GPIO_PIN7
}pin_index_t;

typedef enum{
	PORTA_INDEX = 0,
	PORTB_INDEX,
	PORTC_INDEX,
	PORTD_INDEX
}port_index_t;

typedef struct{
	uint8 port : 3;      /* @ref port_index_t */
	uint8 pin : 3;       /* @ref pin_index_t */
	uint8 direction : 1; /* @ref direction_t */
	uint8 logic : 1;     /* @ref logic_t */
}pin_config_t;

/* Section: Function Declarations */

Std_ReturnType gpio_pin_direction_intialize(const pin_config_t *_pin_config);
Std_ReturnType gpio_pin_get_direction_status(const pin_config_t *_pin_config, direction_t *direction_status);
Std_ReturnType gpio_pin_write_logic(const pin_config_t *_pin_config, logic_t logic);
Std_ReturnType gpio_pin_read_logic(const pin_config_t *_pin_config, logic_t *logic);
Std_ReturnType gpio_pin_toggle_logic(const pin_config_t *_pin_config);
Std_ReturnType gpio_pin_intialize(const pin_config_t *_pin_config);

Std_ReturnType gpio_port_direction_intialize(port_index_t port, uint8 direction);
Std_ReturnType gpio_port_get_direction_status(port_index_t port, uint8 *direction_status);
Std_ReturnType gpio_port_write_logic(port_index_t port, uint8 logic);
Std_ReturnType gpio_port_read_logic(port_index_t port, uint8 *logic);
Std_ReturnType gpio_port_toggle_logic(port_index_t port);



#endif /* MCAL_GPIO_H_ */
