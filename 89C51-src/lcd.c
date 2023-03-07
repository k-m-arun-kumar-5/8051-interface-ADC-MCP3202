/* ********************************************************************
   FILE                   : lcd.c

   PROGRAM DESCRIPTION    : LCD library 
                      									 
	 
   AUTHOR                : K.M. Arun Kumar alias Arunkumar Murugeswaran
	 
   KNOWN BUGS            : 

   NOTE                  :  										
                                    
   CHANGE LOGS           : 

*****************************************************************************/
#include "main.h"

lcd lcd_unit; 
unsigned int lcd_avail_loc_within_limit = STATE_YES;
unsigned int cur_disp_lcd_loc = BEGIN_LOC_LINE1, cur_input_lcd_loc = BEGIN_LOC_LINE2;
unsigned int lcd_read_command_var;
disp_format_t disp_format_data; 

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Init

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        :    
-*------------------------------------------------------------*/
void LCD_Init()
{
	unsigned int lcd_func_command;
			
	#if (LCD_DATA_INTERFACE == DATA_INTERFACE_4_BITS)
		lcd_unit.conf.interface = DATA_INTERFACE_4_BITS;	   
	#else
		lcd_unit.conf.interface = DATA_INTERFACE_8_BITS;
	#endif
	
	#if (LCD_BUSY_FLAG == CHECK_BUSY_FLAG)
		lcd_unit.conf.check_bf = CHECK_BUSY_FLAG;
	  /*  #ifdef TRACE
		   UART_Transmit_Str("LCD check BUSY Flag \r");
		#endif */
		ERROR_LED = 1;
		return; 
	#else
		lcd_unit.conf.check_bf = NO_CHECK_BUSY_FLAG;
	  /*   #ifdef TRACE
		   UART_Transmit_Str("LCD dont check BUSY Flag \r");
		#endif */
	#endif	
	LCD_DATA_PORT = 0x00; 
	//  wait for more than 15ms after LCD VSS rises to 4.5V, Busy Flag in LCD (BF) cannot be checked 
	Delay_in_Milli_Sec(1500UL);
    LCD_Write_Command_NO_BF(0x30);
	// wait for more than 4.1 ms, Busy Flag in LCD (BF) cannot be checked 
	Delay_in_Milli_Sec(10);
    LCD_Write_Command_NO_BF(0x30);
	// wait for more than 100 us, Busy Flag in LCD (BF) cannot be checked 
	Delay_in_Milli_Sec(1);
    LCD_Write_Command_NO_BF(0x30);
	Delay_in_Milli_Sec(1);
	if(lcd_unit.conf.interface == DATA_INTERFACE_4_BITS)
	{		
	   LCD_Write_Command_NO_BF(0x20); 	  
	} 
	lcd_func_command = 0x20;
	Write_Bit_in_Data(&lcd_func_command , 4, lcd_unit.conf.interface);
	Write_Bit_in_Data(&lcd_func_command , 3, LCD_MAX_DISP_NUM_LINES);
	Write_Bit_in_Data(&lcd_func_command , 2, LCD_CHAR_FONT);
	LCD_Write_Command(lcd_func_command);    
	//LCD_Write_Command(0x28);
		
    /* #ifdef TRACE
	   to_disp.unsigned_val.value_long = lcd_func_command;
       UART_Transmit_Str("LCD Func Set: 0x");
	   UART_Transmit_Num(DISP_HEX_DIGIT2, to_disp);
	   UART_Transmit_Char('\r');
	#endif */
	   
   	LCD_Write_Command(0x08);
	LCD_Clear_Screen();
	LCD_Write_Command(0x06);  
	LCD_Write_Command(0x0E);
	
	return; 
} 
/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Clear_Screen

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        :   
-*------------------------------------------------------------*/
void LCD_Clear_Screen()
{
	LCD_Write_Command(0x01);
	Delay_in_Micro_Sec(LCD_CLEAR_EXEC_IN_USEC);	
	return; 
}
 /*------------------------------------------------------------*
FUNCTION NAME  : LCD_Write_Command

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        :   
-*------------------------------------------------------------*/
void LCD_Write_Command_NO_BF(const unsigned int lcd_cmd)
{
	//after LCD is not busy 
 	Delay_in_Milli_Sec(1);
		
    LCD_RW = 0;
    LCD_RS = 0;
   
    Delay_in_Micro_Sec(LCD_ADDR_SETUP_IN_USEC);   
    LCD_EN = 1;
    Delay_in_Micro_Sec(LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC);
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (lcd_cmd & 0xF0); 
	Delay_in_Micro_Sec(LCD_DATA_SETUP_IN_USEC);
    LCD_EN = 0;
       //SHOULD_REMOVE
       //UART_Transmit_Char(LCD_DATA_PORT);
	
    Delay_in_Micro_Sec(LCD_OTHER_INST_EXEC_IN_USEC); 	
	return; 
   
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Write_Command

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        :   
-*------------------------------------------------------------*/
void LCD_Write_Command(const unsigned int lcd_cmd)
{
	//after LCD is not busy 
 	Delay_in_Milli_Sec(1);
		
   LCD_RW = 0;
   LCD_RS = 0;
   
    Delay_in_Micro_Sec(LCD_ADDR_SETUP_IN_USEC);   
    LCD_EN = 1;
    Delay_in_Micro_Sec(LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC);
	if(lcd_unit.conf.interface == DATA_INTERFACE_4_BITS)
	{
	    // send higher nibble first and higher port nibble of LCD PORT is connected to LCD(DB4 - DB7)	 
        LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (lcd_cmd & 0xF0); 
	    Delay_in_Micro_Sec(LCD_DATA_SETUP_IN_USEC);
        LCD_EN = 0;
        //SHOULD_REMOVE
        //UART_Transmit_Char(LCD_DATA_PORT);
        Delay_in_Micro_Sec(LCD_OTHER_INST_EXEC_IN_USEC); 	 
   
        Delay_in_Micro_Sec(LCD_ADDR_SETUP_IN_USEC);   
        LCD_EN = 1;
        Delay_in_Micro_Sec(LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC);
	    // send lower nibble and higher port nibble of LCD PORT is connected to LCD(DB4 - DB7)	 
        LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | ((lcd_cmd << 4) & 0xF0); 
	}
    else
	{	
        LCD_DATA_PORT = (lcd_cmd); 
	}
	 Delay_in_Micro_Sec(LCD_DATA_SETUP_IN_USEC);
     LCD_EN = 0;
       //SHOULD_REMOVE
       //UART_Transmit_Char(LCD_DATA_PORT);
	
    Delay_in_Micro_Sec(LCD_OTHER_INST_EXEC_IN_USEC); 	
	return; 
   
}
/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Write_Data

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        :    
-*------------------------------------------------------------*/
 void LCD_Write_Data(const char lcd_data)
{
	//after LCD is not busy 
	Delay_in_Milli_Sec(1);
	LCD_RW = 0;
    LCD_RS = 1;
	
	Delay_in_Micro_Sec(LCD_ADDR_SETUP_IN_USEC);   
    LCD_EN = 1;
    Delay_in_Micro_Sec(LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC);
	if(lcd_unit.conf.interface == DATA_INTERFACE_4_BITS)
	{
	    // send higher nibble first and higher port nibble of LCD PORT must be connected to LCD(DB4 - DB7)	 	 
        LCD_DATA_PORT =  (LCD_DATA_PORT & 0x0F) | (lcd_data & 0xF0); 
	    Delay_in_Micro_Sec(LCD_DATA_SETUP_IN_USEC);
        LCD_EN = 0;
        //SHOULD_REMOVE
        //UART_Transmit_Char(LCD_DATA_PORT);
      Delay_in_Micro_Sec (LCD_OTHER_INST_EXEC_IN_USEC); 	 
   
       Delay_in_Micro_Sec (LCD_ADDR_SETUP_IN_USEC);   
       LCD_EN  = 1;
       Delay_in_Micro_Sec(LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC);
	   // send lower nibble and higher port nibble of LCD PORT must be connected to LCD (DB4 - DB7)		 
       LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | ((lcd_data << 4) & 0xF0);  
	}
	else
	{
		LCD_DATA_PORT = (lcd_data ); 
	}
	  Delay_in_Micro_Sec(LCD_DATA_SETUP_IN_USEC);
    LCD_EN = 0;
    //SHOULD_REMOVE
    //UART_Transmit_Char(LCD_DATA_PORT);
    Delay_in_Micro_Sec(LCD_OTHER_INST_EXEC_IN_USEC);
   return; 	
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Disp_Str

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        :    
-*------------------------------------------------------------*/
void LCD_Disp_Str(const char *char_ptr)
{ 
       char cur_char;
	   
       while(*char_ptr)
       {
		    cur_char = *(char_ptr++);
            LCD_Write_Data(cur_char);
			//UART_Transmit_Char(cur_char);
       }
	   //UART_Transmit_Char('\r');   
      return; 	   
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Disp_Num

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        :   
-*------------------------------------------------------------*/
 void LCD_Disp_Num(const disp_format_t lcd_disp_data)
{
    unsigned int tens_thousand_digit,thousands_digit, hundreds_digit,tens_digit, unit_digit;
	unsigned long disp_num, num ;
	char num_data[] ={'0','1','2','3','4','5','6','7','8','9'};  
	char hex_data[] ={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'}; 
		
	if(lcd_disp_data.disp_sign  == STATE_YES) //signed lcd_datanum_disp_format
	{
		if(lcd_disp_data.disp_data < 0 )
		{
			LCD_Write_Data('-');
			disp_num = -lcd_disp_data.disp_data;
		}
        else
		{
			LCD_Write_Data('+');
			disp_num = lcd_disp_data.disp_data;
		}        
	}
	else
	{
    	disp_num = lcd_disp_data.disp_data; 
	}
	num =  disp_num ;
	
    switch(lcd_disp_data.num_digits_format)
	{
	  case DISP_NUM_DIGIT5:
	 	  num =  disp_num ;
		  tens_thousand_digit = (unsigned int)(num / (unsigned long)(10000UL));
          LCD_Write_Data(num_data[tens_thousand_digit]);
	  case DISP_NUM_DIGIT4:
	      num = disp_num % 10000UL;
	      thousands_digit = (unsigned int)(num / (unsigned long)(1000UL));
	      LCD_Write_Data(num_data[thousands_digit]); 
	  case DISP_NUM_DIGIT3:
	      num = disp_num % 1000UL;
	      hundreds_digit = (unsigned int) (num / (unsigned long) (100));
	      LCD_Write_Data(num_data[hundreds_digit]);
	  case DISP_NUM_DIGIT2:
	      num = disp_num % 100;
          tens_digit = (unsigned int) (num / 10);
          LCD_Write_Data(num_data[tens_digit]); 		  
	  case DISP_NUM_DIGIT1:
	      unit_digit = (unsigned int) (disp_num % 10);
          LCD_Write_Data(num_data[unit_digit]); 
	  break;
	  case DISP_HEX_DIGIT4:
	      //  ( 16 * 16 * 16 *16 )  = 0 as divide by zero warning 
	      //num = disp_num % ( 16 * 16 * 16 *16 );
          thousands_digit = (num / (unsigned long) (16 * 16 * 16));
	      LCD_Write_Data(hex_data[thousands_digit]);
	  case DISP_HEX_DIGIT3:
	      num = disp_num %(unsigned long)(16 * 16 * 16);
	      hundreds_digit = (unsigned int) (num / (unsigned long) (16 * 16));
	      LCD_Write_Data(hex_data[hundreds_digit]);
	  case DISP_HEX_DIGIT2:
	      num = disp_num %(unsigned long)(16 * 16);
          tens_digit = (unsigned int) (num / 16);
          LCD_Write_Data(hex_data[tens_digit]);
	  case DISP_HEX_DIGIT1: 
	      unit_digit = (unsigned int) (disp_num % 16);
          LCD_Write_Data(hex_data[unit_digit]);    
	  break;
	  
	} 
    return;   	
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Disp_Goto_XY

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        :    
-*------------------------------------------------------------*/
void LCD_Disp_Goto_XY(const unsigned int start_line_num, const unsigned int start_col_num)
{
	/* max 4 lines and 20 columns */
	/* for us, lcd col starts from 1, but for uC, col starts from 0 */
    unsigned int start_col_lcd = start_col_num - 1 ; 
	lcd_avail_loc_within_limit = STATE_YES;
	
   if(start_line_num <= CONFIGURE_MAX_NUM_LINES && start_col_num <= CONFIGURE_MAX_NUM_COLS )
   {
     switch(start_line_num)
	 {
		 case NUM_LINE1:
		   cur_disp_lcd_loc = BEGIN_LOC_LINE_NUM_01;
		   break;
		 case NUM_LINE2:
		   cur_disp_lcd_loc = BEGIN_LOC_LINE_NUM_02;
		   break;
		 case NUM_LINE3:
		   cur_disp_lcd_loc = BEGIN_LOC_LINE_NUM_03;
		   break;
		  case NUM_LINE4:
		   cur_disp_lcd_loc = BEGIN_LOC_LINE_NUM_04 ;
		   break;		 
	 }	
      cur_disp_lcd_loc = cur_disp_lcd_loc + start_col_lcd;
      LCD_Write_Command(cur_disp_lcd_loc);       
   }
   else
   {
	   /* error due to invalid lcd DISP loc  */
	     lcd_avail_loc_within_limit = STATE_NO;	
   }
   return;   
} 

/*-------------------------------------------------------------------
  ------------------------ END OF FILE ------------------------------
-------------------------------------------------------------------*/
