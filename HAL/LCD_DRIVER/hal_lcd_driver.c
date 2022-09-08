/*
 * hal_lcd_driver.c
 *
 * Created: 7/7/2022 12:44:16 PM
 *  Author: Ramy
 */ 

#include "hal_lcd_driver.h"

static Std_ReturnType lcd_send_4bits(const lcd_t *_lcd, uint8 _data_command);
static Std_ReturnType lcd_send_8bits(const lcd_t *_lcd, uint8 _data_command);
static Std_ReturnType lcd_set_cursor(const lcd_t *_lcd, uint8 row, uint8 coulmn);


static Std_ReturnType lcd_send_enable_signal(const lcd_t *_lcd);


Std_ReturnType lcd_intialize(const lcd_t *_lcd){
	Std_ReturnType ret = E_OK;
	if(NULL == _lcd){
		ret = E_NOT_OK;
	}
	else{
		
		#if _lcd_mode_bit_set == 4
		
		// pin initialization  
		ret = gpio_pin_intialize(&(_lcd->lcd_enable));
		ret = gpio_pin_intialize(&(_lcd->lcd_rs));
		
		for (uint8 pin_counter=0; pin_counter<4; pin_counter++)
		{
			ret = gpio_pin_intialize(&(_lcd->lcd_data[pin_counter])); // ret = gpio_pin_intialize(&(_lcd->lcd_data[0]));
		}
		
		// Main command send 
		_delay_ms(100);
		ret = lcd_send_command((_lcd),_lcd_8bit_mode_2_line);
		_delay_ms(5);
		ret = lcd_send_command((_lcd),_lcd_8bit_mode_2_line);
		_delay_us(150);
		ret = lcd_send_command((_lcd),_lcd_8bit_mode_2_line);
		
		// secondary command configuration 
		
		
		ret = lcd_send_command((_lcd),_lcd_clear_display);
		ret = lcd_send_command((_lcd),_lcd_return_home);
		ret = lcd_send_command((_lcd),_lcd_entry_mode);
		ret = lcd_send_command((_lcd),_lcd_cursor_off_display_on);
		ret = lcd_send_command((_lcd),_lcd_4bit_mode_2_line);
		ret = lcd_send_command((_lcd),_lcd_DDRAM_row1);
		
		
		
		#elif _lcd_mode_bit_set == 8
		
		
		// pin initialization
		ret = gpio_pin_intialize(&(_lcd->lcd_enable));
		ret = gpio_pin_intialize(&(_lcd->lcd_rs));
		for (int pin_counter=0; pin_counter<8; pin_counter++)
		{
			ret = gpio_pin_intialize(&(_lcd->lcd_data[pin_counter]));
		}
		
		// command send
		_delay_ms(20);
		ret = lcd_send_command(&(_lcd),_lcd_8bit_mode_2_line);
		_delay_ms(5);
		ret = lcd_send_command(&(_lcd),_lcd_8bit_mode_2_line);
		_delay_us(100);
		ret = lcd_send_command(&(_lcd),_lcd_8bit_mode_2_line);
		
		// secondary command configuration
		
		ret = lcd_send_command((_lcd),_lcd_clear_display);
		ret = lcd_send_command((_lcd),_lcd_return_home);
		ret = lcd_send_command((_lcd),_lcd_entry_mode);
		ret = lcd_send_command((_lcd),_lcd_cursor_off_display_on);
		ret = lcd_send_command((_lcd),_lcd_8bit_mode_2_line);
		ret = lcd_send_command((_lcd),_lcd_DDRAM_row1);
		

      #endif
		
	}
	return ret;
}

Std_ReturnType lcd_send_command(const lcd_t *_lcd, uint8 command){
	Std_ReturnType ret = E_OK;
	if(NULL == _lcd){
		ret = E_NOT_OK;
	}
	else{
		
			#if _lcd_mode_bit_set == 4
			
			ret = gpio_pin_write_logic(&(_lcd->lcd_rs),GPIO_LOW);
			ret = lcd_send_4bits(_lcd,command>>4);
			ret = lcd_send_enable_signal((_lcd));
			ret = lcd_send_4bits(_lcd,command);
			ret = lcd_send_enable_signal((_lcd));
			
		//	_delay_ms(5);
		
			#elif _lcd_mode_bit_set == 8
			
			ret = gpio_pin_write_logic(&(_lcd->lcd_rs),GPIO_LOW);
			ret = lcd_send_8bits(&(_lcd),command);
			ret = lcd_send_enable_signal(&(_lcd));

			#endif
	}
	return ret;
}

Std_ReturnType lcd_send_char_data(const lcd_t *_lcd, uint8 data){
	Std_ReturnType ret = E_OK;
	if(NULL == _lcd){
		ret = E_NOT_OK;
	}
	else{
		
		#if _lcd_mode_bit_set == 4
		
		ret = gpio_pin_write_logic(&(_lcd->lcd_rs),GPIO_HIGH);
		ret = lcd_send_4bits(_lcd,data>>4);
		ret = lcd_send_enable_signal((_lcd));
		ret = lcd_send_4bits(_lcd,data);
		ret = lcd_send_enable_signal(_lcd);
		
		
		#elif _lcd_mode_bit_set == 8
		
		ret = gpio_pin_write_logic(&(_lcd->lcd_rs),GPIO_HIGH);
		ret = lcd_send_8bits(&(_lcd),data);
		ret = lcd_send_enable_signal(&(_lcd));

		#endif
	}
	return ret;
}

Std_ReturnType lcd_send_char_data_pos(const lcd_t *_lcd, uint8  row , uint8 coloumn , uint8 data){
	Std_ReturnType ret = E_OK;
	if(NULL == _lcd){
		ret = E_NOT_OK;
	}
	else{
		
		#if _lcd_mode_bit_set == 4
		
		ret = lcd_set_cursor(_lcd,row,coloumn);
		
		ret = lcd_send_char_data(_lcd,data);
		
		#elif _lcd_mode_bit_set == 8
		
		ret = lcd_set_cursor(_lcd,row,coloumn);
		
		ret = lcd_send_char_data(_lcd,data);

		#endif
	}
	return ret;
}

Std_ReturnType lcd_send_string_data(const lcd_t *_lcd, uint8 *str){
	Std_ReturnType ret = E_OK;
	if(NULL == _lcd || NULL == str)
	{
		ret = E_NOT_OK;
	}
	else{
		
		while(*str)
		{
			ret = lcd_send_char_data(_lcd , *str++) ;
			
			 
		}
		
	}
	return ret;
}

Std_ReturnType lcd_send_string_data_pos(const lcd_t *_lcd, uint8  row , uint8 coloumn , uint8 *str){
	Std_ReturnType ret = E_OK;
	if(NULL == _lcd || NULL == str){
		ret = E_NOT_OK;
	}
	else{
		
		ret = lcd_set_cursor(_lcd,row,coloumn);
		
			while(*str)
			{
				ret = lcd_send_char_data(_lcd , *str++) ;
			}
	}
	return ret;
}


static Std_ReturnType lcd_send_4bits(const lcd_t *_lcd, uint8 _data_command){
	
	Std_ReturnType ret = E_OK;
	
	ret = gpio_pin_write_logic(&(_lcd->lcd_data[0]),( _data_command>>0 ) & (uint8)0x01 );
	ret = gpio_pin_write_logic(&(_lcd->lcd_data[1]),( _data_command>>1 ) & (uint8)0x01 );
	ret = gpio_pin_write_logic(&(_lcd->lcd_data[2]),( _data_command>>2 ) & (uint8)0x01 );
	ret = gpio_pin_write_logic(&(_lcd->lcd_data[3]),( _data_command>>3 ) & (uint8)0x01 );

	
	return ret;
}

static Std_ReturnType lcd_send_8bits(const lcd_t *_lcd, uint8 _data_command){
	Std_ReturnType ret = E_OK;
	for (int pin_counter=0; pin_counter<8; pin_counter++)
	{
		ret = gpio_pin_write_logic(&(_lcd->lcd_data[pin_counter]),( _data_command>>pin_counter & (uint8) 0x01 ));
	}
	
	return ret;
}

static Std_ReturnType lcd_send_enable_signal(const lcd_t *_lcd){
	Std_ReturnType ret = E_OK;
	ret = gpio_pin_write_logic(&(_lcd->lcd_enable), GPIO_HIGH);
	_delay_ms(5);
	ret = gpio_pin_write_logic(&(_lcd->lcd_enable), GPIO_LOW);
	return ret;
}

static Std_ReturnType lcd_set_cursor(const lcd_t *_lcd, uint8 row, uint8 coulmn){
	
	Std_ReturnType ret = E_OK;
	coulmn--;
	
	switch (row)
	
	{
		
	case 1 : ret = lcd_send_command((_lcd),(_lcd_DDRAM_row1 + coulmn));  break;
	case 2 : ret = lcd_send_command((_lcd),(_lcd_DDRAM_row2 + coulmn));  break;
	case 3 : ret = lcd_send_command((_lcd),(_lcd_DDRAM_row3 + coulmn));  break;
	case 4 : ret = lcd_send_command((_lcd),(_lcd_DDRAM_row4 + coulmn));  break;
	
		default : ;
	}
	
	return ret;
}

Std_ReturnType convert_uint8_to_string(uint8 value, uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }
    else{    
        memset((char *)str, '\0', 4);
        sprintf(str, "%i", value);
    }
    return ret;
}

/**
 * 
 * @param value
 * @param str
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType convert_uint16_to_string(uint16 value, uint8 *str){
	Std_ReturnType ret = E_OK;
	uint8 Temp_String[6] = {0};
	uint8 DataCounter = 0;
	if(NULL == str){
		ret = E_NOT_OK;
	}
	else{
		memset(str, ' ', 5);
		str[5] = '\0';
		sprintf((char *)Temp_String, "%i", value);
		while(Temp_String[DataCounter] != '\0'){
			str[DataCounter] = Temp_String[DataCounter];
			DataCounter++;
		}
	}
	return ret;
}

/**
 * 
 * @param value
 * @param str
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType convert_uint32_to_string(uint32 value, uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }
    else{    
        memset((char *)str, '\0', 11);
        sprintf(str, "%i", value);
    }
    return ret;
}

Std_ReturnType lcd_send_uint16_data_pos(const lcd_t *_lcd, uint8  row , uint8 coloumn , uint16 int_value){
	Std_ReturnType ret = E_OK;
	if(NULL == _lcd ){
		ret = E_NOT_OK;
	}
	else{
			uint8 temp_buffer[7]={'\0'};
			uint8 index=0;
			uint8 str[7]={' ',' ',' ',' ',' ',' '}; // for uint32 use str[12]
			
			utoa(int_value,temp_buffer,10);
				 while(temp_buffer[index]!= '\0' )
				 {
					str[index]=temp_buffer[index];
					index++;
				 }
			

		ret = lcd_send_string_data_pos(_lcd,row,coloumn,&str);
	}
	return ret;
}

Std_ReturnType lcd_send_uint8_data_pos(const lcd_t *_lcd, uint8  row , uint8 coloumn , uint8 int_value){
	Std_ReturnType ret = E_OK;
	if(NULL == _lcd ){
		ret = E_NOT_OK;
	}
	else{
		
		uint8 temp_buffer[5]={'\0'};
		uint8 str[5]={' ',' ',' ',' '};
		uint8 index=0;
	
		utoa(int_value,temp_buffer,10);
		
		while(temp_buffer[index]!= '\0' ){
			str[index]=temp_buffer[index];
			index++;
		}
		

		ret = lcd_send_string_data_pos(_lcd,row,coloumn,&str);
	}
	return ret;
}

Std_ReturnType lcd_send_uint32_data_pos(const lcd_t *_lcd, uint8  row , uint8 coloumn , uint16 int_value){
	Std_ReturnType ret = E_OK;
	if(NULL == _lcd ){
		ret = E_NOT_OK;
	}
	else{
		uint8 temp_buffer[12]={'\0'};
		uint8 index=0;
		uint8 str[12]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}; // for uint32 use str[12]
		
		utoa(int_value,temp_buffer,10);
		while(temp_buffer[index]!= '\0' )
		{
			str[index]=temp_buffer[index];
			index++;
		}
		

		ret = lcd_send_string_data_pos(_lcd,row,coloumn,&str);
	}
	return ret;
}