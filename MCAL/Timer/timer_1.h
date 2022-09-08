/*
 * timer_1.h
 *
 * Created: 8/11/2022 11:39:21 PM
 *  Author: Ramy
 */ 


#ifndef TIMER_1_H_
#define TIMER_1_H_

/* Section : Includes */

#include "../mcal_gpio/mcal_gpio.h"
#include "../../LIB/register_mem_map.h"

/* Section: Macro Declarations */

#define  Interrupt_Global_Enable()    SET_BIT(SREG_reg ,7)

/* Compare match modes non_pwm */

#define t1_OC1A_disconnect			 0x00
#define t1_OC1A_toggle		         0x01
#define t1_OC1A_set	                 0x02
#define t1_OC1A_clear	             0x03

#define t1_OC1B_disconnect			 0x00
#define t1_OC1B_toggle		         0x01
#define t1_OC1B_set	                 0x02
#define t1_OC1B_clear	             0x03


#define TCNT1_reg (*( volatile uint16 *)0x4C)
#define OCR1A_reg (*( volatile uint16 *)0x4A)
#define OCR1B_reg (*( volatile uint16 *)0x48)
#define ICR1_reg  (*( volatile uint16 *)0x46)


typedef union {
	
	volatile uint8 TCCR1A_full_reg ;
	
	struct {
		volatile uint8 WGM_10_11	 : 2;
		volatile uint8 FOC1_A        : 1;
		volatile uint8 FOC1_B	  	 : 1;
		volatile uint8 COM1B		 : 2;
		volatile uint8 COM1A		 : 2;
		
	};
}TCCR1A_union;

TCCR1A_union TCCR1A_reg;

#define TCCR1A_reg (*(TCCR1A_union*)0x4F)

/* ----------------------------------*/

typedef union {
	
	volatile uint8 TCCR1B_full_reg ;
	
	struct {
		volatile uint8 CS_10to12_bits	 : 3;
		volatile uint8 WGM_12_13		 : 2;
		volatile uint8 reserved_bit	  	 : 1;
		volatile uint8 ICES1_bit		 : 1;
		volatile uint8 ICNC1_bit		 : 1;
		
	};
}TCCR1B_union;

TCCR1B_union TCCR1B_reg;

#define TCCR1B_reg (*(TCCR1B_union*)0x4E)

/*---------------------------------*/

#define t1_Capture_Noise_Canceler_enable() TCCR1B_reg.ICNC1_bit = 0x01 ;
#define t1_Noise_Cancellation_enable			0x01
#define t1_Noise_Cancellation_disable			0x00

 
#define t1_Capture_edge_failing()          TCCR1B_reg.ICES1_bit = 0x00 ;
#define t1_Capture_edge_rising()           TCCR1B_reg.ICES1_bit = 0x01 ;

#define t1_Capture_failing_edge			   0x00

#define t1_Capture_rising_edge             0x01

	
/* Section: Macro Functions Declarations */

#define t1_ovf_interrupt_enable()				SET_BIT(TIMSK,TOIE1);
#define t1_ovf_interrupt_disable()			    CLEAR_BIT(TIMSK,TOIE1);
#define t1_ovf_interrupt_Flag_check()			READ_BIT(TIFR,TOV1);
#define t1_ovf_interrupt_Flag_clear()		    SET_BIT(TIFR,TOV1);

#define t1_com_A_interrupt_enable()			    SET_BIT(TIMSK,OCIE1A);
#define t1_com_A_interrupt_disable()		    CLEAR_BIT(TIMSK,OCIE1A);
#define t1_com_A_interrupt_Flag_check()			READ_BIT(TIFR,OCF1A);
#define t1_com_A_interrupt_Flag_clear()		    SET_BIT(TIFR,OCF1A);

#define t1_com_B_interrupt_enable()			    SET_BIT(TIMSK,OCIE1B);
#define t1_com_B_interrupt_disable()		    CLEAR_BIT(TIMSK,OCIE1B);
#define t1_com_B_interrupt_Flag_check()			READ_BIT(TIFR,OCF1B);
#define t1_com_B_interrupt_Flag_clear()		    SET_BIT(TIFR,OCF1B);

#define t1_input_cap_interrupt_enable()		    SET_BIT(TIMSK,TICIE1);
#define t1_input_cap_interrupt_disable()        CLEAR_BIT(TIMSK,TICIE1);
#define t1_input_cap_interrupt_Flag_check()	    READ_BIT(TIFR,ICF1);
#define t1_input_cap_interrupt_Flag_clear()	    SET_BIT(TIFR,ICF1);

/* Section: Data Type Declarations */



typedef enum {
	t1_OCR1A,
	t1_OCR1B
}t1_output_pin_slc;

typedef enum {
	t1_OC1A_OC1B_dis =0,
	t1_OC1A_toggle_OC1A_OC1B_dis, // on FAST PWM WGM = 15 Toggle OCA1 ... on PHASE CORRECT WGM = 9 or 14 Toggle OCA1 else OCA1-B1 Disconnect.
	t1_OC1A_OC1B_toggle_non_inverting,
	t1_OC1A_OC1B_toggle_inverting
	
}timer1_Pin_output_mode;

typedef enum {
	t1_Normal = 0,
	t1_PWM_phase_correct_10bit=3,
	t1_ctc_OCR1A=4,
	t1_fast_pwm_10bit=7,
	t1_PWM_phase_freq_correct_ICR1=8,
	t1_PWM_phase_freq_correct_OCR1A=9,
	t1_PWM_phase_correct_ICR1=10,
	t1_PWM_phase_correct_OCR1A=11,
	t1_ctc_ICR1=12,
	t1_fast_pwm_ICR1=14,	
	t1_fast_pwm_OCR1A=15

}timer1_WG_mode;

typedef enum {
	t1_clk_No = 0,
	t1_clk_full,
	t1_clk_8,
	t1_clk_64,
	t1_clk_256,
	t1_clk_1024,
	t1_clk_falling_edge_t1_pin,
	t1_clk_rising_edge_t1_pin
	
}timer1_prescaler_slc;

typedef struct {
	
	void (*t1_ovf_interrupt_handler)(void) ;
	void (*t1_com_A_interrupt_handler)(void) ;
	void (*t1_com_B_interrupt_handler)(void) ;
	void (*t1_input_cap_interrupt_handler)(void) ;
	timer1_prescaler_slc prescaler_slc;
	timer1_WG_mode wave_gen_mode_slc;
	timer1_Pin_output_mode t1_OC1A_pin_mode;
	timer1_Pin_output_mode t1_OC1B_pin_mode;
	uint8 t1_Capture_slc : 2 ; 	
	uint8 t1_Capture_noise_cancel : 2 ; 
	uint8 reserved : 4 ;

	
}timer_1_t;

/* Section: Function Declarations */

Std_ReturnType timer1_init(const timer_1_t * t1_obj);
Std_ReturnType timer1_deinit(const timer_1_t * t1_obj);

Std_ReturnType timer1_input_Capture_edge_select( uint8 edge_slc);
Std_ReturnType timer1_input_Capture_PWM_specs(const timer_1_t * t1_obj, uint8 * frequency , uint8 * duty_cycle);

Std_ReturnType timer1_fast_PWM_Generation(const timer_1_t * t1_obj, t1_output_pin_slc pin , uint32 frequency_hz , uint8 duty_cycle);
//Std_ReturnType timer1_phase_correct_PWM_Generation(const timer_1_t * t1_obj, uint8 frequency , uint8 duty_cycle);
Std_ReturnType timer1_Capture_PWM_Messure(const timer_1_t * t1_obj, uint32 *frequency_hz , uint8 *duty_cycle);
Std_ReturnType timer1_Capture_PWM_Messure2(const timer_1_t * t1_obj, uint32 *frequency_hz , uint8 *duty_cycle);


Std_ReturnType timer1_OC1A_reg_wirte(const timer_1_t * t1_obj, uint16 value);
Std_ReturnType timer1_OC1A_reg_read(const timer_1_t * t1_obj, uint16 * result);

Std_ReturnType timer1_OC1B_reg_wirte(const timer_1_t * t1_obj, uint16 value);
Std_ReturnType timer1_OC1B_reg_read(const timer_1_t * t1_obj, uint16 * result);



#endif /* TIMER_1_H_ */
