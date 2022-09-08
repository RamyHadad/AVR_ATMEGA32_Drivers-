/*
 * interrupt_external.c
 *
 * Created: 7/18/2022 2:44:36 PM
 *  Author: Ramy
 */ 

/* Section : Includes */

#include "interrupt_external.h"

/* Section: Function Declarations */


static InterruptHandler INT0_InterruptHandler = NULL;  // InterruptHandler is a type def pointer to function 
static InterruptHandler INT1_InterruptHandler = NULL;
static InterruptHandler INT2_InterruptHandler = NULL;

//static Std_ReturnType logic_sense_polling(const EX_interrupt_t * int_obj);


static Std_ReturnType EX_interrupt_INTX_pin_init(const EX_interrupt_t * int_obj);
static Std_ReturnType EX_interrupt_INTX_enable(const EX_interrupt_t * int_obj);
static Std_ReturnType EX_interrupt_INTX_disable(const EX_interrupt_t * int_obj);
static Std_ReturnType EX_interrupt_INTX_sense_select(const EX_interrupt_t * int_obj);
//static Std_ReturnType EX_interrupt_INTX_clear_flag(const EX_interrupt_t * int_obj);

static Std_ReturnType InterruptHandler_INT0(void (*InterruptHandler)(void));
static Std_ReturnType InterruptHandler_INT1(void (*InterruptHandler)(void));
static Std_ReturnType InterruptHandler_INT2(void (*InterruptHandler)(void));
static Std_ReturnType InterruptHandler_INTX(const EX_interrupt_t * int_obj);




/* Section: Macro Functions Declarations */

/* Section: Function Definition */


Std_ReturnType EX_interrupt_intialize(const EX_interrupt_t * int_obj){
Std_ReturnType ret = E_OK;
if(NULL == int_obj){
	ret = E_NOT_OK;
}
else{
		if (!(READ_BIT(SREG_reg ,7))) // enable global interrupt if it was turned off
		{
			Interrupt_Global_Enable();
		}
		// disable external interrupt 
		 ret = EX_interrupt_INTX_disable(int_obj);
		// initialize external interrupt pins 
		 ret = EX_interrupt_INTX_pin_init(int_obj);
		// initialize external interrupt sense mode 
		 ret = EX_interrupt_INTX_sense_select(int_obj);
		// initialize user defined function address to the static pointer to function 
		 ret = InterruptHandler_INTX (int_obj);
		// enable external interrupt
	  	 ret =  EX_interrupt_enable(int_obj);
	
}

return ret;

}

Std_ReturnType EX_interrupt_enable(const EX_interrupt_t * int_obj){
Std_ReturnType ret = E_OK;
if(NULL == int_obj){
	ret = E_NOT_OK;
}
else{
	
	ret = EX_interrupt_INTX_enable(int_obj);
	ret = E_OK ;
}

return ret;


}

Std_ReturnType EX_interrupt_disable(const EX_interrupt_t * int_obj){
Std_ReturnType ret = E_OK;
if(NULL == int_obj){
	ret = E_NOT_OK;
}
else{
	
	ret = EX_interrupt_INTX_disable(int_obj);
	ret = E_OK ; 
}

return ret;


}

static Std_ReturnType EX_interrupt_INTX_pin_init(const EX_interrupt_t * int_obj){
Std_ReturnType ret = E_OK;
if(NULL == int_obj){
	ret = E_NOT_OK;
}
else{
	
	ret = gpio_pin_intialize(&(int_obj->pins));
	
	ret = E_OK;
}

return ret;
	
}

static Std_ReturnType EX_interrupt_INTX_enable(const EX_interrupt_t * int_obj){
	Std_ReturnType ret = E_OK;
	if(NULL == int_obj){
		ret = E_NOT_OK;
	}
	else{
		
		switch(int_obj->source)
		{
			case INTERRUPT_EXTERNAL_INT0 :
			 Ex_Interrupt_INT0_Enable() ;
			 ret = E_OK;
			  break;
			
			case INTERRUPT_EXTERNAL_INT1 :
			 Ex_Interrupt_INT1_Enable() ;
			 ret = E_OK;
			  break;
			
			case INTERRUPT_EXTERNAL_INT2 : 
			Ex_Interrupt_INT2_Enable() ;
			ret = E_OK;
			 break;
		}
		
		
	}

	return ret;
	
}

static Std_ReturnType EX_interrupt_INTX_disable(const EX_interrupt_t * int_obj){
	Std_ReturnType ret = E_OK;
	if(NULL == int_obj){
		ret = E_NOT_OK;
	}
	else{
		
		switch(int_obj->source)
		{
			case INTERRUPT_EXTERNAL_INT0 :
			Ex_Interrupt_INT0_Disable() ;
			ret = E_OK;
			break;
			
			case INTERRUPT_EXTERNAL_INT1 :
			Ex_Interrupt_INT1_Disable() ;
			ret = E_OK;
			break;
			
			case INTERRUPT_EXTERNAL_INT2 :
			Ex_Interrupt_INT2_Disable() ;
			ret = E_OK;
			break;
			
			default: ret = E_NOT_OK;
		}
		
		
	}

	return ret;
	
}

static Std_ReturnType EX_interrupt_INTX_sense_select(const EX_interrupt_t * int_obj){
	Std_ReturnType ret = E_OK;
	if(NULL == int_obj){
		ret = E_NOT_OK;
	}
	else{
		
		switch(int_obj->source)
		{
			 case INTERRUPT_EXTERNAL_INT0 :			 
			 switch(int_obj->sense_logic)
			 {
				 case INT_LOW_LEVEL :    Ex_Interrupt_INT0_low_level()    ;  ret = E_OK;  break;
				 case INT_LOGIC_CHANGE : Ex_Interrupt_INT0_logic_change() ;  ret = E_OK;  break;
				 case INT_FALLING_EDGE : Ex_Interrupt_INT0_falling_edge() ;	 ret = E_OK;  break;
				 case INT_RISING_EDGE :  Ex_Interrupt_INT0_rising_edge()  ;  ret = E_OK;  break;
				 default: ret = E_NOT_OK; break;
			 }
			
			 break;
			 
			 case INTERRUPT_EXTERNAL_INT1 :
			 
			 switch(int_obj->sense_logic)
			 {
				 case INT_LOW_LEVEL :    Ex_Interrupt_INT1_low_level()    ; ret = E_OK; break;
				 case INT_LOGIC_CHANGE : Ex_Interrupt_INT1_logic_change() ; ret = E_OK; break;
				 case INT_FALLING_EDGE : Ex_Interrupt_INT1_falling_edge() ; ret = E_OK; break;
				 case INT_RISING_EDGE :  Ex_Interrupt_INT1_rising_edge()  ; ret = E_OK; break;
				 default: ret = E_NOT_OK; break;
			 }

			 break;
			
			case INTERRUPT_EXTERNAL_INT2 :
			
			switch(int_obj->sense_logic)
			{
				case INT_FALLING_EDGE : Ex_Interrupt_INT2_falling_edge() ; ret = E_OK; break;
				case INT_RISING_EDGE :  Ex_Interrupt_INT2_rising_edge()  ; ret = E_OK; break;
				default: ret = E_NOT_OK; break;
			}
			
			ret = E_OK;
			break;
			
			default: ret = E_NOT_OK;
		}
	}

	return ret;
}

static Std_ReturnType InterruptHandler_INT0(void (*InterruptHandler)(void)){
	Std_ReturnType ret = E_OK;
	if(NULL == InterruptHandler){
		ret = E_NOT_OK;
	}
	
	else
	
	{
		 INT0_InterruptHandler = InterruptHandler ;
		 ret = E_OK;
	}
	
	return ret;
}

static Std_ReturnType InterruptHandler_INT1(void (*InterruptHandler)(void)){
	Std_ReturnType ret = E_OK;
	if(NULL == InterruptHandler){
		ret = E_NOT_OK;
	}
	
	else
	
	{
		INT1_InterruptHandler = InterruptHandler ;
		ret = E_OK;
	}
	
	return ret;
}

static Std_ReturnType InterruptHandler_INT2(void (*InterruptHandler)(void)){
	Std_ReturnType ret = E_OK;
	if(NULL == InterruptHandler){
		ret = E_NOT_OK;
	}
	
	else
	
	{
		INT2_InterruptHandler = InterruptHandler ;
		ret = E_OK;
	}
	
	return ret;
}

static Std_ReturnType InterruptHandler_INTX(const EX_interrupt_t * int_obj){
	Std_ReturnType ret = E_OK;
	if(NULL == int_obj){
		ret = E_NOT_OK;
	}
	else
	{
		switch(int_obj->source)
		{
			case INTERRUPT_EXTERNAL_INT0 :
			ret= InterruptHandler_INT0(int_obj->EXTERNAL_INTERRUPT_HANDLER) ; // INT0_InterruptHandler = int_obj->EXTERNAL_INTERRUPT_HANDLER ;
			ret = E_OK;
			break;
			
			case INTERRUPT_EXTERNAL_INT1 :
			ret= InterruptHandler_INT1(int_obj->EXTERNAL_INTERRUPT_HANDLER) ;
			ret = E_OK;
			break;
			
			case INTERRUPT_EXTERNAL_INT2 :
			ret= InterruptHandler_INT2(int_obj->EXTERNAL_INTERRUPT_HANDLER) ;
			ret = E_OK;
			break;
		}
		
		ret = E_OK;
	}
	
	return ret;
}

void INT0_ISR(void){
	
	// calling the function stored in the corresponding pointer to function 
	 if(INT0_InterruptHandler){ INT0_InterruptHandler(); }
		 else{ /* Nothing */ }
	
}

void INT1_ISR(void){
	
	// calling the function stored in the corresponding pointer to function
	if(INT1_InterruptHandler){ INT1_InterruptHandler(); }
	else{ /* Nothing */ }
	
}

void INT2_ISR(void){
	
	// calling the function stored in the corresponding pointer to function
	if(INT2_InterruptHandler){ INT2_InterruptHandler(); }
	else{ /* Nothing */ }
	
}

/*static Std_ReturnType logic_sense_polling_INT0(const EX_interrupt_t * int_obj){
	Std_ReturnType ret = E_OK;
	if(NULL == int_obj){
		ret = E_NOT_OK;
	
	switch(int_obj->sense_logic)
	{
		case INT_LOW_LEVEL    : Ex_Interrupt_INT0_low_level()    ;  break;
		case INT_LOGIC_CHANGE : Ex_Interrupt_INT0_logic_change() ;  break;
		case INT_FALLING_EDGE : Ex_Interrupt_INT0_falling_edge() ;  break;
		case INT_RISING_EDGE  : Ex_Interrupt_INT0_rising_edge()  ;  break;
		default: ret = E_NOT_OK; break;
	}
	
		return ret;
}
*/

 ISR(INT0_vect){
	
	 INT0_InterruptHandler();
	
}
 ISR(INT1_vect){
	
	INT1_InterruptHandler();
	
}
 ISR(INT2_vect){
	
	INT2_InterruptHandler();
	
}