/*
 * mcal_register_map.h
 *
 * Created: 7/20/2022 11:19:42 PM
 *  Author: Ramy
 */ 


#ifndef MCAL_REGISTER_MAP_H_
#define MCAL_REGISTER_MAP_H_

#include "../LIB/std_types.h"

/* Section: EX_Interrupt registers */

#define  SREG_reg	*(( volatile uint8 *) 0x5F )
#define  GIFR_reg	*(( volatile uint8 *) 0x5A )
#define  GICR_reg	*(( volatile uint8 *) 0x5B )
#define  MCUCSR_reg	*(( volatile uint8 *) 0x54 )
#define  MCUCR_reg	*(( volatile uint8 *) 0x55 )




#endif /* MCAL_REGISTER_MAP_H_ */