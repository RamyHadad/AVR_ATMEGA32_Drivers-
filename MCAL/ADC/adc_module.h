/*
 * adc_module.h
 *
 * Created: 7/24/2022 10:06:10 PM
 *  Author: Ramy
 */ 


#ifndef ADC_MODULE_H_
#define ADC_MODULE_H_

/* Section : Includes */

#include "../mcal_gpio/mcal_gpio.h"
#include "adc_config.h"
#include "../../LIB/register_mem_map.h"

/* Section: Macro Declarations */

#define ADCLH (*(volatile uint16 *)0x24) // address for adsl casted to uint16 so that both adcl & adch are combined 

//#define  ADMUX_LSB_nibble_mask  0xF0 

typedef union {
	
	volatile uint8 ADMUX_8bit ;
	
	struct  {
		
		volatile uint8   MUX	: 5 ; // channel slx bits combined 
		volatile uint8	ADLAR_5 : 1 ;
		volatile uint8	REFS0_6 : 1 ;
		volatile uint8	REFS1_7 : 1 ;
	};
	
}ADMUX_union;

ADMUX_union ADMUX_reg;

#define ADMUX_reg (*(ADMUX_union*)0x27) 



/* Section: Macro Functions Declarations */

#define adc_left_adjust()             SET_BIT(ADMUX,ADLAR)
#define adc_right_adjust()            CLEAR_BIT(ADMUX,ADLAR) 

#define adc_enable()                  SET_BIT(ADCSRA,ADEN)
#define adc_disable()                 CLEAR_BIT(ADCSRA,ADEN)

#define _adc_start_conversion()         SET_BIT(ADCSRA,ADSC)
#define _adc_conversion_check()         READ_BIT(ADCSRA,ADSC)
#define _adc_clear_flag()			    SET_BIT(ADCSRA,ADIF)


#define adc_auto_trigger_enable()     SET_BIT(ADCSRA,ADATE)   // a source selected by set a ADTS in SFIOR
#define adc_auto_trigger_disable()    CLEAR_BIT(ADCSRA,ADATE)

#define  Interrupt_Global_Enable()    SET_BIT(SREG_reg ,7)
#define adc_interrupt_enable()        SET_BIT(ADCSRA,ADIE)
#define adc_interrupt_disable()       CLEAR_BIT(ADCSRA,ADIE)

#define adc_volt_ref_AREF()          do { CLEAR_BIT(ADMUX,REFS1);CLEAR_BIT(ADMUX,REFS0);} while(0)
#define adc_volt_ref_AVCC()			 do { CLEAR_BIT(ADMUX,REFS1);SET_BIT(ADMUX,REFS0);} while(0)
#define adc_volt_ref_internal()      do { SET_BIT(ADMUX,REFS1);SET_BIT(ADMUX,REFS0);} while(0)


/* Section: Data Type Declarations */

typedef enum {
	AREF_source=0,
	AVCC_source,
	internal_source=3
	
}adc_voltage_reference ;

typedef enum{
	divison_2 =0 ,
	divison_2_re,
	divison_4,
	divison_8,
	divison_16,
	divison_32,
	divison_64,
	divison_128
	
}adc_prescaler_slc;

typedef enum {
	ADC0 = 0,
	ADC1,
	ADC2,
	ADC3,
	ADC4,
	ADC5,
	ADC6,
	ADC7
	
}adc_channel_slc;

typedef enum {
	
	adc_free_running =0,
	adc_analog_compartor,
	adc_ex_interrupt_0,
	adc_timer0_compare_match,
	adc_timer0_over_flow,
	adc_timer1_compare_match_B,
	adc_timer1_over_flow,
	adc_timer1_capture_event

}adc_auto_trigger_slc;

typedef struct {

#if adc_interrupt == adc_interrupt_ON

void (*adc_interrupt_handler)(void) ;

#endif	

adc_voltage_reference volt_source;

adc_channel_slc       channel_slc;

adc_prescaler_slc  	prescaler_slc;

#if adc_auto_trigger == adc_triger_ON

adc_auto_trigger_slc auto_trigger_source ;

#endif	

	
}adc_t;



/* Section: Function Declarations */

Std_ReturnType adc_module_init(const adc_t * adc_obj);
Std_ReturnType adc_module_deinit(const adc_t * adc_obj);
Std_ReturnType adc_select_channel(const adc_t * adc_obj , adc_channel_slc channel);
Std_ReturnType adc_start_conversion(const adc_t * adc_obj);
Std_ReturnType adc_conversion_status(const adc_t * adc_obj, uint8 *status);
Std_ReturnType adc_result (const adc_t * adc_obj, uint16 *result);
Std_ReturnType adc_result_blocking (const adc_t * adc_obj, adc_channel_slc channel ,uint16 *result);





#endif /* ADC_MODULE_H_ */
