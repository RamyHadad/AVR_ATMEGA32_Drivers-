/*
 * timer_1.c
 *
 * Created: 8/11/2022 11:39:34 PM
 *  Author: Ramy
 */ 
/* Section : Includes */

#include "timer_1.h"

/* Section: Macro Declarations */

static volatile  uint8 WGM_value;

static volatile  uint8 capture_flag;
static volatile uint16 capture_t1;
static volatile  uint16 capture_t2;
static volatile  uint16 capture_t3;


static uint16 pre_scale_value;

/* Section: Macro Functions Declarations */

static Std_ReturnType t1_wave_gen_select(const timer_1_t * t1_obj);
static void pre_scale_factor(const timer1_prescaler_slc pre_scaler, uint16 * value);

static Std_ReturnType t1_OC1A_pin_select(const timer_1_t * t1_obj);
static Std_ReturnType t1_OC1B_pin_select(const timer_1_t * t1_obj);

static void t1_pre_scaler_select(const timer1_prescaler_slc pre_scaler);

static Std_ReturnType t1_ovf_Interrupt_Handler (void (*InterruptHandler)(void));
static Std_ReturnType t1_com_A_Interrupt_Handler (void (*InterruptHandler)(void));
static Std_ReturnType t1_com_B_Interrupt_Handler (void (*InterruptHandler)(void));
static Std_ReturnType t1_input_cap_Interrupt_Handler (void (*InterruptHandler)(void));
static void t1_Capture_INT_API(void);
static void t1_Capture_INT_API2(void);



/* Section: Data Type Declarations */

static InterruptHandler t1_ovf_InterruptHandler_ptr = NULL;
static InterruptHandler t1_com_A_InterruptHandler_ptr = NULL;
static InterruptHandler t1_com_B_InterruptHandler_ptr = NULL;
static InterruptHandler t1_input_cap_InterruptHandler_ptr = NULL;


/* Section: Function Declarations */

Std_ReturnType timer1_init(const timer_1_t * t1_obj){
	Std_ReturnType ret = E_OK;
	if(NULL == t1_obj){
		ret = E_NOT_OK;
	}
	else{
		
		WGM_value = (t1_obj->wave_gen_mode_slc) ;
		
		/* timer 1 wave generation mode select */
		ret= t1_wave_gen_select(t1_obj);
		
		/* timer 1 OC1A pin mode */
		ret = t1_OC1A_pin_select(t1_obj);
		
		/* timer 1 OC1B pin mode */
		ret = t1_OC1B_pin_select(t1_obj);
		
		/* timer 1 overflow interrupt function */
		ret = t1_ovf_Interrupt_Handler(t1_obj->t1_ovf_interrupt_handler);
		
		/* timer 1 OC1A compare match interrupt function */
		ret = t1_com_A_Interrupt_Handler(t1_obj->t1_com_A_interrupt_handler);
		
		/* timer 1 OC1B compare match interrupt function */
		ret = t1_com_B_Interrupt_Handler(t1_obj->t1_com_B_interrupt_handler);
		
		/* timer input capture interrupt function */
		ret = t1_input_cap_Interrupt_Handler(t1_obj->t1_input_cap_interrupt_handler);
		
		/* timer 1 prescaler select */
		t1_pre_scaler_select(t1_obj->prescaler_slc);
		
		/* timer 1 capture mode edge select */
		ret = timer1_input_Capture_edge_select(t1_obj->t1_Capture_slc);
		/* timer 1 capture mode noise cancellation enable */
		if (t1_obj->t1_Capture_noise_cancel == 1)
		{
			t1_Capture_Noise_Canceler_enable();
		}
		else {/*Nothing*/};
		if (t1_obj->t1_input_cap_interrupt_handler != NULL)
		{
					t1_input_cap_interrupt_enable();
		}
		else {/*Nothing*/};
		
		
		
		ret = E_OK;
		
		
	}

	return ret;
}

Std_ReturnType timer1_deinit(const timer_1_t * t1_obj){
	Std_ReturnType ret = E_OK;
	if(NULL == t1_obj){
		ret = E_NOT_OK;
	}
	else{
		
		/* timer 0 prescaler select */
		t1_pre_scaler_select(t1_clk_No);
		/* timer 0 interrupt disable */
		t1_ovf_interrupt_disable();
		t1_com_A_interrupt_disable();
		t1_com_B_interrupt_disable();

		
		ret = E_OK;
	}

	return ret;
}

Std_ReturnType timer1_input_Capture_edge_select(uint8 edge_slc){
	Std_ReturnType ret = E_OK;
	if( edge_slc > 1){
		ret = E_NOT_OK;
	}
	else{
		if (edge_slc == t1_Capture_rising_edge) 
		{
			t1_Capture_edge_rising();
		} 
		else if (edge_slc == t1_Capture_failing_edge)
		{
			t1_Capture_edge_failing();
		}
		else {/*NOTHING*/}
		
		
		ret = E_OK;
		
	}

	return ret;
}

static Std_ReturnType t1_wave_gen_select(const timer_1_t * t1_obj){
	
	Std_ReturnType ret = E_OK;
	if(NULL == t1_obj){
		ret = E_NOT_OK;
	}
	else{
		
		TCCR1A_reg.WGM_10_11 = ( WGM_value & 0x03) ; 
		TCCR1B_reg.WGM_12_13 = ( ( WGM_value & 0x0C)>>2 ) ;
		
	
	}

	return ret;
	
}

static Std_ReturnType t1_OC1A_pin_select(const timer_1_t * t1_obj){
	
	Std_ReturnType ret = E_OK;
	if(NULL == t1_obj){
		ret = E_NOT_OK;
	}
	else{ 
		
		if( (t1_obj->t1_OC1A_pin_mode == 0) || t1_obj->t1_OC1A_pin_mode > 3 )// check for non_pwm mode
			{
				TCCR1A_reg.COM1A = t1_OC1A_disconnect ; // default setting
			}
		
		else if ( ( t1_obj->t1_OC1A_pin_mode == 1) )
			{										//  where OC1A will toggle ( needs to be output )
				if( WGM_value == 9 || WGM_value == 14 || WGM_value == 15 || WGM_value == 4 || WGM_value == 12 )
				{
					TCCR1A_reg.COM1A = t1_obj->t1_OC1A_pin_mode ;
					SET_BIT (DDRD,5); // setting  OC1A pin to be output
				}
				else
				{
					TCCR1A_reg.COM1A = t1_obj->t1_OC1A_pin_mode ;
				}
			}
	
		else
			{
				TCCR1A_reg.COM1A = t1_obj->t1_OC1A_pin_mode ;
				SET_BIT (DDRD,5); // setting  OC1A pin to be output
			}
		
		}

	return ret;
	
}

static Std_ReturnType t1_OC1B_pin_select(const timer_1_t * t1_obj){
	
	Std_ReturnType ret = E_OK;
	if(NULL == t1_obj){
		ret = E_NOT_OK;
	}
	else{
			if( (t1_obj->t1_OC1B_pin_mode == 0) || t1_obj->t1_OC1B_pin_mode > 3 )// check for non_pwm mode
			{
				TCCR1A_reg.COM1B = t1_OC1B_disconnect ; // default setting
			}
			else if ( ( t1_obj->t1_OC1B_pin_mode == 1) )
			{										//  where OC1B will toggle ( needs to be output )
				if(  WGM_value == 4 || WGM_value == 12 )
				{
					TCCR1A_reg.COM1B = t1_obj->t1_OC1B_pin_mode ;
					SET_BIT (DDRD,4); // setting  OC1A pin to be output
				}
				else
				{
					TCCR1A_reg.COM1B = t1_obj->t1_OC1B_pin_mode ;
				}
			}
		
			else
			{
				TCCR1A_reg.COM1B = t1_obj->t1_OC1B_pin_mode ;
				SET_BIT (DDRD,4); // setting  OC1A pin to be output
			}
		ret = E_OK;
	}

	return ret;
	
}

Std_ReturnType timer1_OC1A_reg_wirte(const timer_1_t * t1_obj, uint16 value){
	
	Std_ReturnType ret = E_OK;
	if(NULL == t1_obj){
		ret = E_NOT_OK;
	}
	else
	{
		OCR1A_reg = (uint16)value;

	}
		return ret;
}

Std_ReturnType timer1_OC1A_reg_read(const timer_1_t * t1_obj, uint16 * result){
	
	Std_ReturnType ret = E_OK;
	if(NULL == t1_obj){
		ret = E_NOT_OK;
	}
	else
	{
		*result =(uint16) OCR1A_reg ;
	}
	return ret;
}

Std_ReturnType timer1_OC1B_reg_wirte(const timer_1_t * t1_obj, uint16 value){
	
	Std_ReturnType ret = E_OK;
	if(NULL == t1_obj){
		ret = E_NOT_OK;
	}
	else
	{
		OCR1B_reg = (uint16)value;
	}
	return ret;
}

Std_ReturnType timer1_OC1B_reg_read(const timer_1_t * t1_obj, uint16 * result){
	
	Std_ReturnType ret = E_OK;
	if(NULL == t1_obj){
		ret = E_NOT_OK;
	}
	else
	{
		* result =(uint16) OCR1B_reg ; 
	}
	return ret;
}

Std_ReturnType timer1_fast_PWM_Generation(const timer_1_t * t1_obj, t1_output_pin_slc pin , uint32 frequency_hz , uint8 duty_cycle){
	
	Std_ReturnType ret = E_OK;
	
	if(NULL == t1_obj){
		ret = E_NOT_OK;
	}
	else
	{			uint16 top_value = 0 ;
				float pwm_period = 0 ;
				float timer_freq = 0;
				float timer_period = 0;
				uint8 duty_cycle_cal = 0;

				/*switch(WGM_value)
				{
					case 5  : top_value = 0x00FF ; break;
					case 6	: top_value = 0x01FF ; break;
					case 7  : top_value = 0x03FF ; break;
					case 14 : top_value = ICR1_reg ; break;
					default: return ret ; break;
					
				}*/
				/* enhancement check for wgm mode ( no fpwm for mode 15 )
				apply for overflow fixed mode ( probably with interrupt call back )
				apply for inverted & non-inverted 
				*/
				pre_scale_factor(t1_obj->prescaler_slc,&pre_scale_value);
				timer_freq = (float)(Processor_clock/pre_scale_value);
				timer_period = 1/(float)timer_freq;
				pwm_period = 1/(float)frequency_hz;
				duty_cycle_cal = duty_cycle;
				if (duty_cycle_cal >100)
				{duty_cycle_cal = 100;
				}
				if(pwm_period > timer_period )
				{
					if (pin == t1_OCR1A)
					{
						top_value =(float)(pwm_period/timer_period) ;
						ICR1_reg = top_value-1;
						OCR1A_reg = (uint32)( top_value * duty_cycle_cal / 100 );
						/*checking ovf flag then reinitialize timer counter */
						while (!(ICF1));
						t1_input_cap_interrupt_Flag_clear();
						
					}
					else if (pin == t1_OCR1B)
					{
						top_value =(float)(pwm_period/timer_period) ;
						ICR1_reg = top_value;
						OCR1B_reg = (uint32)( top_value * duty_cycle_cal / 100 );
						/*checking ovf flag then reinitialize timer counter */
						while (!(ICF1));
						t1_input_cap_interrupt_Flag_clear();
					}
					else {/*Nothing*/}
				} 
				else
				{	ret = E_NOT_OK ;
				}
				
			ret = E_OK;
	}
	return ret;
}

Std_ReturnType timer1_Capture_PWM_Messure(const timer_1_t * t1_obj, uint32 *frequency_hz , uint8 *duty_cycle){
	
	Std_ReturnType ret = E_OK;
	
	if(NULL == t1_obj){
		ret = E_NOT_OK;
	}
	else
	{						capture_flag = 0;
							TCNT1_reg = 0 ;
				/*-----------------Variables ----------------*/
				uint16 time_period = 0;
				float singe_tick = 0;
				/*-----------------code ----------------*/
				pre_scale_factor(t1_obj->prescaler_slc,&pre_scale_value);
				singe_tick = (float) pre_scale_value/Processor_clock ;
				t1_input_cap_Interrupt_Handler(t1_Capture_INT_API);
				ret = timer1_input_Capture_edge_select(t1_Capture_rising_edge);
				t1_input_cap_interrupt_enable();
				while(capture_flag<3);
				time_period = (capture_t3-capture_t1); 
				*duty_cycle = 100 * (capture_t2-capture_t1) /(float)time_period  ;
				*frequency_hz = ( 1/ ( (float) time_period * singe_tick ) ) ;
				
			ret = E_OK;
	}
	return ret;
}
void t1_Capture_INT_API(void){
	
	if (capture_flag == 0)
	{	capture_t1 = ICR1_reg;
		timer1_input_Capture_edge_select(t1_Capture_failing_edge);
		capture_flag = 1 ;
	} 
	else if (capture_flag == 1)
	{
		capture_t2 = ICR1_reg;
		timer1_input_Capture_edge_select(t1_Capture_rising_edge);
		capture_flag = 2 ;
	}
	else if (capture_flag == 2)
	{
		capture_t3 = ICR1_reg;
		t1_input_cap_interrupt_disable();
		capture_flag = 3 ;
	}
	else {/*NOTHING*/}
	
}

Std_ReturnType timer1_Capture_PWM_Messure2(const timer_1_t * t1_obj, uint32 *frequency_hz , uint8 *duty_cycle){
	
	Std_ReturnType ret = E_OK;
	
	if(NULL == t1_obj){
		ret = E_NOT_OK;
	}
	else
	{						capture_flag = 0;
		TCNT1_reg = 0 ;
		/*-----------------Variables ----------------*/
		uint16 time_period = 0;
		float singe_tick = 0;
		/*-----------------code ----------------*/
		t1_pre_scaler_select(t1_obj->prescaler_slc);
		pre_scale_factor(t1_obj->prescaler_slc,&pre_scale_value);
		t1_input_cap_Interrupt_Handler(t1_Capture_INT_API2);
		ret = timer1_input_Capture_edge_select(t1_Capture_rising_edge);
		TCNT1_reg = 0 ;
		t1_input_cap_interrupt_enable();
		while(capture_flag<3);
		if ((capture_t1 < capture_t2) && (capture_t2 < capture_t3))
		{
			time_period = (capture_t3-capture_t1);
			*duty_cycle = 100 * (capture_t2-capture_t1) / (float)(time_period ) ;
			*frequency_hz = ( Processor_clock / ( (float) time_period ) * (pre_scale_value) ) ;
		}
		else {*frequency_hz=123456;}
		
		
		ret = E_OK;
	}
	return ret;
}
void t1_Capture_INT_API2(void){
	
	if (capture_flag == 0)
	{	capture_t1 = ICR1_reg;
		timer1_input_Capture_edge_select(t1_Capture_failing_edge);
		capture_flag = 1 ;
	}
	else if (capture_flag == 1)
	{
		capture_t2 = ICR1_reg;
		timer1_input_Capture_edge_select(t1_Capture_rising_edge);
		capture_flag = 2 ;
	}
	else if (capture_flag == 2)
	{
		capture_t3 = ICR1_reg;
		t1_input_cap_interrupt_disable();
		t1_pre_scaler_select(0);
		capture_flag = 3 ;
	}
	else {/*NOTHING*/}
}

static void t1_pre_scaler_select(const timer1_prescaler_slc pre_scaler){
	
	TCCR1B_reg.CS_10to12_bits = pre_scaler ;
}
static void pre_scale_factor(const timer1_prescaler_slc pre_scaler, uint16 * value){
	
	uint16 pre_fact = 0 ; 
	switch(pre_scaler)
	{
		case t1_clk_full :   pre_fact =  1;		break;
		
		case t1_clk_8 :		 pre_fact = 8;	 break;
		
		case t1_clk_64 :	 pre_fact = 64;	 break;
		
		case t1_clk_256 :    pre_fact = 256;    break;
		
		case t1_clk_1024 :   pre_fact = 1024;    break;
		
		default: break;
		
	}
	*value = pre_fact ;
	
}

static Std_ReturnType t1_ovf_Interrupt_Handler (void (*InterruptHandler)(void)){
	
	Std_ReturnType ret = E_OK;
	if(NULL == InterruptHandler){
		ret = E_NOT_OK;
	}
	else{
		
		t1_ovf_interrupt_disable();
		
		t1_ovf_InterruptHandler_ptr = InterruptHandler ;
		t1_ovf_interrupt_enable();
		
		if (!(READ_BIT(SREG_reg ,7))) // enable global interrupt if it was turned off
		{
			Interrupt_Global_Enable();
		}

	}
	

	return ret;
	
}
static Std_ReturnType t1_com_A_Interrupt_Handler (void (*InterruptHandler)(void)){
	
	Std_ReturnType ret = E_OK;
	if(NULL == InterruptHandler){
		ret = E_NOT_OK;
	}
	else{
		
		t1_com_A_InterruptHandler_ptr = InterruptHandler;
		
		t1_com_A_interrupt_enable();
		
		if (!(READ_BIT(SREG_reg ,7))) // enable global interrupt if it was turned off
		{
			Interrupt_Global_Enable();
		}

	}

	return ret;
	
}
static Std_ReturnType t1_com_B_Interrupt_Handler (void (*InterruptHandler)(void)){
	
	Std_ReturnType ret = E_OK;
	if(NULL == InterruptHandler){
		ret = E_NOT_OK;
	}
	else{
		
		t1_com_B_InterruptHandler_ptr = InterruptHandler;
		
		t1_com_B_interrupt_enable();
		
		if (!(READ_BIT(SREG_reg ,7))) // enable global interrupt if it was turned off
		{
			Interrupt_Global_Enable();
		}

	}

	return ret;
	
}
static Std_ReturnType t1_input_cap_Interrupt_Handler (void (*InterruptHandler)(void)){
	
	Std_ReturnType ret = E_OK;
	if(NULL == InterruptHandler){
		ret = E_NOT_OK;
	}
	else{
		
		t1_input_cap_InterruptHandler_ptr = InterruptHandler;
		
		if (!(READ_BIT(SREG_reg ,7))) // enable global interrupt if it was turned off
		{
			Interrupt_Global_Enable();
		}

	}

	return ret;
	
}




ISR(TIMER1_COMPA_vect){
	
	t1_com_A_InterruptHandler_ptr();
	
}

ISR(TIMER1_COMPB_vect){
	
	t1_com_B_InterruptHandler_ptr();
	
}

ISR(TIMER1_OVF_vect){
	
	t1_ovf_InterruptHandler_ptr();
	
}

ISR(TIMER1_CAPT_vect){
	
	t1_input_cap_InterruptHandler_ptr();
	
}