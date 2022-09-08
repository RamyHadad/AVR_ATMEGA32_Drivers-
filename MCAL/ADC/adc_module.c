/*
 * adc_module.c
 *
 * Created: 7/24/2022 10:06:23 PM
 *  Author: Ramy
 */ 


/* Section : Includes */

#include "adc_module.h"

/* Section: Macro Declarations */

static InterruptHandler adc_InterruptHandler = NULL;

/* Section: Macro Functions Declarations */

static Std_ReturnType adc_volt_ref_slc (const adc_t * adc_obj);
static Std_ReturnType adc_pre_scaler_slc (const adc_t * adc_obj);
static Std_ReturnType adc_auto_trig_slc (const adc_t * adc_obj);

static Std_ReturnType adc_Interrupt_Handler (void (*InterruptHandler)(void));

/* Section: Data Type Declarations */

/* Section: Function Definition */

Std_ReturnType adc_module_init(const adc_t * adc_obj){
Std_ReturnType ret = E_OK;
	if(NULL == adc_obj){
		ret = E_NOT_OK;
	}
	else{
		
		// disable ADC module 
			adc_disable();
		// configure voltage reference for ADC module 
			ret = adc_volt_ref_slc(adc_obj);
		// configure default channel for ADC module
		uint8 channel = adc_obj->channel_slc ; 
			ret = adc_select_channel(adc_obj,channel);
		// configure prescaler for ADC module
		ret = adc_pre_scaler_slc(adc_obj);	
		// configure auto trigger
		#if adc_auto_trigger == adc_triger_ON
		
			adc_auto_trigger_enable();
			
			ret = adc_auto_trig_slc(adc_obj);
		 
		#endif
		
		adc_right_adjust(); 
		
		#if adc_interrupt == adc_interrupt_ON
		// disable interrupt before configuration
		adc_interrupt_disable();
		if (!(READ_BIT(SREG_reg ,7))) // enable global interrupt if it was turned off
		{
			Interrupt_Global_Enable();
		}
		ret = adc_Interrupt_Handler(adc_obj->adc_interrupt_handler);
		
		adc_interrupt_enable();
		
		#endif
		
		// Clear ADC flag
				_adc_clear_flag();
			//	ADCL = 0x00 ; 
				
		// enable ADC module
		adc_enable();		
	  ret = E_OK;

	}

	return ret;

}
Std_ReturnType adc_module_deinit(const adc_t * adc_obj){
	Std_ReturnType ret = E_OK;
	if(NULL == adc_obj){
		ret = E_NOT_OK;
	}
	else{
		
		 /* Disable the ADC */
		 adc_disable();
		 /* Disable the interrupt */
		 adc_interrupt_disable();
		        ret = E_OK;

	}

	return ret;

}
Std_ReturnType adc_select_channel(const adc_t * adc_obj , adc_channel_slc channel){
	Std_ReturnType ret = E_OK;
	if(NULL == adc_obj){
		ret = E_NOT_OK;
	}
	else{
		// change the ADMUX reg bit 0-4 to slc the channel 
		ADMUX_reg.MUX = channel ; 
		// ADMUX &= ADMUX_LSB_nibble_mask;
		// ADMUX |= channel ;
       ret = E_OK;

	}

	return ret;

}
Std_ReturnType adc_start_conversion(const adc_t * adc_obj){
	Std_ReturnType ret = E_OK;
	if(NULL == adc_obj){
		ret = E_NOT_OK;
	}
	else{
		
		_adc_start_conversion();
       ret = E_OK;

	}

	return ret;

}
Std_ReturnType adc_conversion_status(const adc_t * adc_obj, uint8 *status){
	Std_ReturnType ret = E_OK;
	if(NULL == adc_obj || NULL == status ){
		ret = E_NOT_OK;
	}
	else{
		
		*status = _adc_conversion_check(); 
		ret = E_OK ; 

	}

	return ret;

}
Std_ReturnType adc_result (const adc_t * adc_obj, uint16 *result){
	Std_ReturnType ret = E_OK;
	if(NULL == adc_obj || NULL == result ){
		ret = E_NOT_OK;
	}
	else{
		
		 *result = ADCLH ; 
		
		//*result = (uint16) ( ADCL + (ADCH<<8)) ;
		
		ret = E_OK ;
	}

	return ret;

}
Std_ReturnType adc_result_blocking (const adc_t * adc_obj, adc_channel_slc channel ,uint16 *result){
	Std_ReturnType ret = E_OK;
	if(NULL == adc_obj || NULL == result ){
		ret = E_NOT_OK;
	}
	else{
		 /* select the A/D channel */
		 ret = adc_select_channel(adc_obj, channel);
		 /* Start the conversion */
		 ret = adc_start_conversion(adc_obj);
		 /* Check if conversion is completed */
		 while((_adc_conversion_check()));
		 ret = adc_result(adc_obj, result);
		 ret = E_OK ;

	}

	return ret;

}

static Std_ReturnType adc_volt_ref_slc (const adc_t * adc_obj){
	
	Std_ReturnType ret = E_OK;
	if(NULL == adc_obj){
		ret = E_NOT_OK;
	}
	else{
		
		switch(adc_obj->volt_source) 
		{
			case AREF_source :		adc_volt_ref_AREF();	    ret = E_OK;	break;
			case AVCC_source :		adc_volt_ref_AVCC();		ret = E_OK;	break;
			case internal_source :  adc_volt_ref_internal();	ret = E_OK;	break;

		}
		       
	}

	return ret;
	
}
static Std_ReturnType adc_pre_scaler_slc (const adc_t * adc_obj){
	
	Std_ReturnType ret = E_OK;
	if(NULL == adc_obj){
		ret = E_NOT_OK;
	}
	
	else{
			ADCSRA |= adc_obj->prescaler_slc ; 
			       ret = E_OK;

	}

	return ret;
	
}
static Std_ReturnType adc_auto_trig_slc (const adc_t * adc_obj){
	
	Std_ReturnType ret = E_OK;
	if(NULL == adc_obj){
		ret = E_NOT_OK;
	}
	
	else{
		
		#if adc_auto_trigger == adc_triger_ON

		SFIOR |= ( (adc_obj->auto_trigger_source) << 5 ) ;
				  ret = E_OK;
		#endif

	}

	return ret;
	
}

static Std_ReturnType adc_Interrupt_Handler (void (*InterruptHandler)(void)){
	Std_ReturnType ret = E_OK;
	if(NULL == InterruptHandler){
		ret = E_NOT_OK;
	}
	
	else
	
	{
		adc_InterruptHandler = InterruptHandler ;
		ret = E_OK;
	}
	
	return ret;
}


ISR(ADC_vect){
	
	adc_InterruptHandler(); 
	
}