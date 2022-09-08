/*
 * timer_0.c
 *
 * Created: 8/5/2022 1:49:00 AM
 *  Author: Ramy
 */ 

/* Section : Includes */

#include "timer_0.h"

/* Section: Macro Declarations */


/* Section: Macro Functions Declarations */

 static Std_ReturnType t0_wave_gen_select(const timer_0_t * t0_obj);
 static Std_ReturnType t0_OC0_pin_select(const timer_0_t * t0_obj);
 static void t0_pre_scaler_select(const timer0_prescaler_slc pre_scaler);
 static void pre_scale_factor(const timer0_prescaler_slc pre_scaler);
 
 static Std_ReturnType t0_ovf_Interrupt_Handler (void (*InterruptHandler)(void));
 static Std_ReturnType t0_comp_Interrupt_Handler (void (*InterruptHandler)(void));

/* Section: Data Type Declarations */

 static uint16 pre_scale_fact;

static InterruptHandler t0_ovf_InterruptHandler_ptr = NULL;
static InterruptHandler t0_comp_InterruptHandler_ptr = NULL;

/* Section: Function Declarations */

Std_ReturnType timer0_init(const timer_0_t * t0_obj){
	Std_ReturnType ret = E_OK;
	if(NULL == t0_obj){
		ret = E_NOT_OK;
	}
	else{
		
		//TCNT0 = 0 ;
		/* timer 0 wave generation mode select */
		ret= t0_wave_gen_select(t0_obj);
		
		/* timer 0 OC0 pin mode */
		ret = t0_OC0_pin_select(t0_obj);
		
		/* timer 0 overflow interrupt function */
		ret = t0_ovf_Interrupt_Handler(t0_obj->t0_ovf_interrupt_handler);
		
		/* timer 0 compare match interrupt function */
		ret = t0_comp_Interrupt_Handler(t0_obj->t0_com_interrupt_handler);
		
		/* timer 0 precaler select */
		 t0_pre_scaler_select(t0_obj->prescaler_slc);
		
		
		ret = E_OK;
		
		
	}

	return ret;
}

Std_ReturnType timer0_deinit(const timer_0_t * t0_obj){
	Std_ReturnType ret = E_OK;
	if(NULL == t0_obj){
		ret = E_NOT_OK;
	}
	else{
		
		/* timer 0 prescaler select */
		 t0_pre_scaler_select(t0_No_clk);
		/* timer 0 interrupt disable */
		t0_ovf_interrupt_disable();
		t0_com_interrupt_disable();
		
		ret = E_OK;
	}

	return ret;
}

Std_ReturnType timer0_PWM_OC0_freq_gen_CTC_mode(const timer_0_t * t0_obj, uint32 PWM_freq_Hz){
	Std_ReturnType ret = E_OK;
	if(NULL == t0_obj){
		ret = E_NOT_OK;
	}
	else{
		if ( (t0_obj->t0_OC0_pin_mode == 1 ) && (TCCR0_reg.WGM00_bit ==0) )
		{	pre_scale_factor(t0_obj->prescaler_slc);
			uint8 temp_ocr = 0;
			uint16 temp_fre_ratio = (Processor_clock/(float)PWM_freq_Hz);
			
			temp_ocr = (temp_fre_ratio / (float) (2 * pre_scale_fact));
		
			timer0_OCR_reg_wirte(t0_obj,temp_ocr);
		}
		
		
		ret = E_OK;
	}

	return ret;
}


Std_ReturnType timer0_timer_reg_wirte(const timer_0_t * t0_obj, uint8 value){
	Std_ReturnType ret = E_OK;
	if(NULL == t0_obj){
		ret = E_NOT_OK;
	}
	else{
		 TCNT0 = value ; 
		
		ret = E_OK;
	}

	return ret;
}

Std_ReturnType timer0_timer_reg_read(const timer_0_t * t0_obj, uint8 * result){
	Std_ReturnType ret = E_OK;
	if(NULL == t0_obj){
		ret = E_NOT_OK;
	}
	else{
		
		*result = TCNT0 ; 
		
		ret = E_OK;
	}

	return ret;
}

Std_ReturnType timer0_OCR_reg_wirte(const timer_0_t * t0_obj, uint8 value){
	Std_ReturnType ret = E_OK;
	if(NULL == t0_obj){
		ret = E_NOT_OK;
	}
	else{
		OCR0 = value ;
		
		ret = E_OK;
	}

	return ret;
}
Std_ReturnType timer0_OCR_reg_read(const timer_0_t * t0_obj, uint8 * result){
	Std_ReturnType ret = E_OK;
	if(NULL == t0_obj){
		ret = E_NOT_OK;
	}
	else{
		
		*result = OCR0 ; 
		
		ret = E_OK;
	}

	return ret;
};

static Std_ReturnType t0_wave_gen_select(const timer_0_t * t0_obj){
	
		Std_ReturnType ret = E_OK;
		if(NULL == t0_obj){
			ret = E_NOT_OK;
		}
		else{
			
			switch(t0_obj->wave_gen_mode_slc){
			
			case t0_Normal: 
			(TCCR0_reg.WGM00_bit = 0 ); (TCCR0_reg.WGM01_bit = 0 ); ret = E_OK; break;
			
			case t0_PWM_phase_correct:
			 (TCCR0_reg.WGM00_bit = 1 ); (TCCR0_reg.WGM01_bit = 0 ); ret = E_OK; break;
			 
			case t0_ctc:
			 (TCCR0_reg.WGM00_bit = 0 ); (TCCR0_reg.WGM01_bit = 1 ); ret = E_OK; break;
			 
			case t0_fast_pwm:
			 (TCCR0_reg.WGM00_bit = 1 ); (TCCR0_reg.WGM01_bit = 1 ); ret = E_OK; break;
			 
			 }
		}

		return ret;
	
}

static Std_ReturnType t0_OC0_pin_select(const timer_0_t * t0_obj){
	 
	 Std_ReturnType ret = E_OK;
	 if(NULL == t0_obj){
		 ret = E_NOT_OK;
	 }
	 else{
		 
		 if (t0_obj->t0_OC0_pin_mode > 3)
		 {
			 // default setting 
			 TCCR0_reg.COM_0to1_bits = t0_OC0_disconnect ;
		 }
		 
		 else{
			 
			 TCCR0_reg.COM_0to1_bits = t0_obj->t0_OC0_pin_mode ;
		 }
		 
		 if (t0_obj->wave_gen_mode_slc != t0_Normal )
		 {
			 if (t0_obj->t0_OC0_pin_mode >= 1)
			 {
				SET_BIT (DDRB,3);
			 }
		 }
		 
	 }

	 return ret;
	 
 }
 
static void t0_pre_scaler_select(const timer0_prescaler_slc pre_scaler){
	
			 TCCR0_reg.CS_0to2_bits = pre_scaler ;
 }
static void pre_scale_factor(const timer0_prescaler_slc pre_scaler){
	   
	   
	   switch(pre_scaler)
	   {
		   case t0_full_clk :   pre_scale_fact = 1;     break;
		   
		   case t0_clk_8 :	   pre_scale_fact = 8;	 break;
		   
		   case t0_clk_64 :	   pre_scale_fact = 64;	 break;
		   
		   case t0_clk_256 :    pre_scale_fact = 256;    break;
		   
		   case t0_clk_1024 :    pre_scale_fact = 1024;    break;
		   
	   }
	   
   }


 static Std_ReturnType t0_ovf_Interrupt_Handler (void (*InterruptHandler)(void)){
	 
	 Std_ReturnType ret = E_OK;
	 if(NULL == InterruptHandler){
		 ret = E_NOT_OK;
	 }
	 else{
		   t0_ovf_InterruptHandler_ptr = InterruptHandler ;
		   
		    t0_ovf_interrupt_enable();
		
		  if (!(READ_BIT(SREG_reg ,7))) // enable global interrupt if it was turned off
		   {
			  Interrupt_Global_Enable();
		   }

		  }
	 

	 return ret;
	 
 }
 static Std_ReturnType t0_comp_Interrupt_Handler (void (*InterruptHandler)(void)){
	 
	 Std_ReturnType ret = E_OK;
	 if(NULL == InterruptHandler){
		 ret = E_NOT_OK;
	 }
	 else{
		 
		 t0_comp_InterruptHandler_ptr = InterruptHandler;
		 
		 t0_com_interrupt_enable();
		 
		 if (!(READ_BIT(SREG_reg ,7))) // enable global interrupt if it was turned off
		 {
			 Interrupt_Global_Enable();
		 }

	 }

	 return ret;
	 
 }

 

ISR(TIMER0_COMP_vect){
	
	t0_comp_InterruptHandler_ptr();
	
}

ISR(TIMER0_OVF_vect){
	
	t0_ovf_InterruptHandler_ptr();
	
}	 