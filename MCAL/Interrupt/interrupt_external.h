/*
 * interrupt_external.h
 *
 * Created: 7/18/2022 2:45:27 PM
 *  Author: Ramy
 */ 


#ifndef INTERRUPT_EXTERNAL_H_
#define INTERRUPT_EXTERNAL_H_

/* Section : Includes */

#include "../mcal_gpio/mcal_gpio.h"
#include "interrupt_configration.h"
#include "../../LIB/register_mem_map.h"
//#include <avr/interrupt.h>

/* Section: Macro Declarations */

#define ISC00			0
#define ISC01			1
#define ISC10			2
#define ISC11			3
#define ISC2			6
#define INT1			7
#define INT0			6
#define INT2			5


/* Section: Macro Functions Declarations */

/* Global_Interrupt */

#define  Interrupt_Global_Enable()            SET_BIT(SREG_reg ,7)


/* INT0 */

#define  Ex_Interrupt_INT0_Enable()            SET_BIT(GICR_reg ,INT0)

#define  Ex_Interrupt_INT0_Disable()           CLEAR_BIT(GICR_reg ,INT0)

//#define  Ex_Interrupt_INT0_flag_clear()

#define  Ex_Interrupt_INT0_low_level()       do { CLEAR_BIT(MCUCR_reg ,ISC01);CLEAR_BIT(MCUCR_reg ,ISC00); }while(0)

#define  Ex_Interrupt_INT0_logic_change()    do { CLEAR_BIT(MCUCR_reg ,ISC01);SET_BIT(MCUCR_reg ,ISC00); }while(0)

#define  Ex_Interrupt_INT0_falling_edge()    do { SET_BIT(MCUCR_reg ,ISC01);CLEAR_BIT(MCUCR_reg ,ISC00); }while(0)

#define  Ex_Interrupt_INT0_rising_edge()    do {  SET_BIT(MCUCR_reg ,ISC01) ;SET_BIT(MCUCR_reg ,ISC00); }while(0)

/* INT1 */

#define  Ex_Interrupt_INT1_Enable()            SET_BIT(GICR_reg ,INT1) 

#define  Ex_Interrupt_INT1_Disable()           CLEAR_BIT(GICR_reg ,INT1) 

//#define  Ex_Interrupt_INT1_flag_clear()

#define  Ex_Interrupt_INT1_low_level()        do { CLEAR_BIT(MCUCR_reg ,ISC11);CLEAR_BIT(MCUCR_reg ,ISC10); } while(0)

#define  Ex_Interrupt_INT1_logic_change()     do { CLEAR_BIT(MCUCR_reg ,ISC11);SET_BIT(MCUCR_reg ,ISC10); } while(0)

#define  Ex_Interrupt_INT1_falling_edge()     do { SET_BIT(MCUCR_reg ,ISC11);CLEAR_BIT(MCUCR_reg ,ISC10); } while(0)

#define  Ex_Interrupt_INT1_rising_edge()      do { SET_BIT(MCUCR_reg ,ISC11);SET_BIT(MCUCR_reg ,ISC10); } while(0)

/* INT2 */

#define  Ex_Interrupt_INT2_Enable()            SET_BIT(GICR_reg ,INT2) 

#define  Ex_Interrupt_INT2_Disable()           CLEAR_BIT(GICR_reg ,INT2) 

//#define  Ex_Interrupt_INT2_flag_clear()

#define  Ex_Interrupt_INT2_falling_edge()      CLEAR_BIT(MCUCSR_reg ,ISC2) 

#define  Ex_Interrupt_INT2_rising_edge()       SET_BIT(MCUCSR_reg ,ISC2)


/* Section: Data Type Declarations */


typedef enum {
	
	INT_LOW_LEVEL =0,
	INT_LOGIC_CHANGE ,
	INT_FALLING_EDGE ,
	INT_RISING_EDGE 
	
}Interrupt_Sense_Control;

typedef enum{
	INTERRUPT_EXTERNAL_INT0 =  0,
	INTERRUPT_EXTERNAL_INT1,
	INTERRUPT_EXTERNAL_INT2
}interrupt_INTx_src;

typedef struct {
	
	void (* EXTERNAL_INTERRUPT_HANDLER )(void); // InterruptHandler EXTERNAL_INTERRUPT_HANDLER ;
	pin_config_t pins;
	interrupt_INTx_src source;
	Interrupt_Sense_Control sense_logic;

}EX_interrupt_t;

/* Section: Function Declarations */

Std_ReturnType EX_interrupt_intialize(const EX_interrupt_t * obj);

Std_ReturnType EX_interrupt_enable(const EX_interrupt_t * obj);

Std_ReturnType EX_interrupt_disable(const EX_interrupt_t * obj);



#endif /* INTERRUPT_EXTERNAL_H_ */
