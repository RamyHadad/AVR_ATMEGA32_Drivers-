/*
 * timer_0.h
 *
 * Created: 8/5/2022 1:48:51 AM
 *  Author: Ramy
 */ 


#ifndef TIMER_0_H_
#define TIMER_0_H_

/* Section : Includes */

#include "../mcal_gpio/mcal_gpio.h"
#include "../../LIB/register_mem_map.h"

/* Section: Macro Declarations */

#define  Interrupt_Global_Enable()    SET_BIT(SREG_reg ,7)

/* Compare match modes non_pwm */

#define t0_OC0_disconnect			 0x00
#define t0_OC0_toggle_non_pwm	     0x01

/* Compare match modes pwm */

#define t0_OC0_disconnect			 0x00
#define t0_OC0_pwm_non_inverting     0x02
#define t0_OC0_pwm_inverting	     0x03


typedef union {
	
		volatile uint8 TCCR0_full_reg ;
		
	struct {
		volatile uint8 CS_0to2_bits  : 3;
		volatile uint8 WGM01_bit	 : 1;
		volatile uint8 COM_0to1_bits : 2;
		volatile uint8 WGM00_bit	 : 1;
		volatile uint8 FOC0_bit		 : 1;
		
		};
}TCCR0_union;

TCCR0_union TCCR0_reg;

#define TCCR0_reg (*(TCCR0_union*)0x53)


/* Section: Macro Functions Declarations */

#define t0_ovf_interrupt_enable()			SET_BIT(TIMSK,TOIE0);
#define t0_ovf_interrupt_disable()          CLEAR_BIT(TIMSK,TOIE0);

#define t0_com_interrupt_enable()		    SET_BIT(TIMSK,OCIE0);
#define t0_com_interrupt_disable()          CLEAR_BIT(TIMSK,OCIE0);

/* Section: Data Type Declarations */


typedef enum {
	t0_Normal = 0,
	t0_PWM_phase_correct,
	t0_ctc,
	t0_fast_pwm
	
}timer0_WG_mode;

typedef enum {
	t0_No_clk = 0,
	t0_full_clk,
	t0_clk_8,
	t0_clk_64,
	t0_clk_256,
	t0_clk_1024,
	t0_clk_falling_edge_t0_pin,
	t0_clk_rising_edge_t0_pin
	
}timer0_prescaler_slc;

typedef struct {
	
	void (*t0_ovf_interrupt_handler)(void) ;
	void (*t0_com_interrupt_handler)(void) ;
	timer0_prescaler_slc prescaler_slc;
	timer0_WG_mode wave_gen_mode_slc;
	uint8 t0_OC0_pin_mode;
	
}timer_0_t;

/* Section: Function Declarations */

Std_ReturnType timer0_init(const timer_0_t * t0_obj);
Std_ReturnType timer0_deinit(const timer_0_t * t0_obj);

Std_ReturnType timer0_PWM_OC0_freq_gen_CTC_mode(const timer_0_t * t0_obj, uint32 PWM_freq_Hz);



Std_ReturnType timer0_timer_reg_wirte(const timer_0_t * t0_obj, uint8 value);
Std_ReturnType timer0_timer_reg_read(const timer_0_t * t0_obj, uint8 * result);


Std_ReturnType timer0_OCR_reg_wirte(const timer_0_t * t0_obj, uint8 value);
Std_ReturnType timer0_OCR_reg_read(const timer_0_t * t0_obj, uint8 * result);




#endif /* TIMER_0_H_ */
