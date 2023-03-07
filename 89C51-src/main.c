/* ********************************************************************
FILE                   : main.c

PROGRAM DESCRIPTION    : Program for the 8051/8052 with SPI interface to ADC module (MCP3202)
                          (Application Task) to read analog voltage in channel 0 of MCP3202 and
						  display analog voltage and percentage in LCD using 4 bit interface. 
                      									 
	 
AUTHOR                : K.M. Arun Kumar alias Arunkumar Murugeswaran
	 
KNOWN BUGS            : 

NOTE                  :  RECOMMENDED to use SPI mode: 0 for MCP3202 ADC Module. 
                         MCU implementation supports only MSB First MCP3202 ADC Conversation. 	   							
                                    
CHANGE LOGS           : 

*****************************************************************************/

#include "main.h"


/*------------------------------------------------------------*-
FUNCTION NAME  : main

DESCRIPTION    :          
								
INPUT          : none

OUTPUT         : 

NOTE           : 
-*------------------------------------------------------------*/

void main(void)
{
	 unsigned char adc_conf = 0; 
     unsigned long cur_adc_result, prev_adc_result = MAX_ADC_VALUE + 1;  	 
	 unsigned long analog_val_in_digital_int_ch0, analog_val_in_digital_frac_ch0;
	 unsigned int percent_int_ch0, percent_frac_ch0;

	 
     /* ========= begin : USER CODE INITIALIZE ========== */
	 ERROR_LED = 0;         //output for MCU
	 ADC_CONSTANT_LED = 0;  //output for MCU
	 LCD_Init();
	 LCD_Const_Disp();
	 Serial_Init();  		
	 putchar(0x0C);    // clear Hyper terminal
	 MCP3202_ADC_Init(); 
	 ADC_CONSTANT_LED = STATE_OFF;
	 ERROR_LED = STATE_OFF;
	 /* ========= end : USER CODE INITIALIZE ========== */
	   
     while(1) // Super Loop
     {
		   	adc_conf = 0; 
		    adc_conf |= (ADC_MSB_FIRST << ADC_MSB_FIRST_MODE_BIT_POS); 
            adc_conf |= (ADC_SGL_MODE << ADC_SGL_DIFF_MODE_BIT_POS); 
            adc_conf |= (ADC_CH_00 << ADC_ODD_SIGN_MODE_BIT_POS); 
		    adc_conf |= (SPI_MODE_0 << ADC_SPI_MODE_BIT_POS);
            cur_adc_result = MCP3202_ADC_Read_Proc(adc_conf); 
            
			if(cur_adc_result != prev_adc_result)
			{
				 #ifdef TRACE
			      printf("Ch 0 : %4lu, 0x%4lX \r", cur_adc_result, cur_adc_result);
				#endif 
				 ADC_CONSTANT_LED = STATE_OFF;
				 
                 Encoded_To_Actual_Analog_Val_Calc(cur_adc_result, FULL_SCALE_ANALOG_VAL_CH0, MIN_ANALOG_VALUE_CH0,\
		               &analog_val_in_digital_int_ch0,&analog_val_in_digital_frac_ch0 );
                 disp_format_data.disp_sign = STATE_NO;
                 disp_format_data.num_digits_format = DISP_NUM_DIGIT2;
                 disp_format_data.disp_data = analog_val_in_digital_int_ch0;	   
                 LCD_Disp_Goto_XY(SIGCH0_LINE_NUM, NUM_COL1);   
                 LCD_Disp_Num(disp_format_data);
		
	    	     disp_format_data.disp_sign = STATE_NO;
                 disp_format_data.num_digits_format = DISP_NUM_DIGIT1;
                 disp_format_data.disp_data = analog_val_in_digital_frac_ch0;
	    	     LCD_Disp_Goto_XY(SIGCH0_LINE_NUM, SIGNAL_FRAC_COL_NUM);		
	             LCD_Disp_Num(disp_format_data);
		
		         Encoded_To_Percent_Calc(cur_adc_result, &percent_int_ch0, &percent_frac_ch0 );
		         disp_format_data.disp_sign = STATE_NO;
                 disp_format_data.num_digits_format = DISP_NUM_DIGIT3;
                 disp_format_data.disp_data = percent_int_ch0;
		         LCD_Disp_Goto_XY(SIGCH0_LINE_NUM, PERCENT_INT_COL_NUM);
	             LCD_Disp_Num(disp_format_data);
		
	             disp_format_data.disp_sign = STATE_NO;
                 disp_format_data.num_digits_format = DISP_NUM_DIGIT1;
		         disp_format_data.disp_data = percent_frac_ch0;
		         LCD_Disp_Goto_XY(SIGCH0_LINE_NUM, PERCENT_FRAC_COL_NUM);		
		         LCD_Disp_Num(disp_format_data);					
				 prev_adc_result = cur_adc_result;
			}
			else
			{
				 ADC_CONSTANT_LED = STATE_ON;
			}
     }
	 return; 
}

/*------------------------------------------------------------*-
FUNCTION NAME  : LCD_Const_Disp

DESCRIPTION    :  
	
INPUT          :  

OUTPUT         : 

NOTE           :  
-*------------------------------------------------------------*/
void LCD_Const_Disp()
{
	const char signal_rep_disp[] = "V";
	
	LCD_Disp_Goto_XY(NUM_LINE1, NUM_COL1);
	LCD_Disp_Str("ADC Channel 0 Output");	       
  LCD_Disp_Goto_XY (SIGCH0_LINE_NUM,SIGNAL_DECPT_COL_NUM );
	LCD_Write_Data('.');
	LCD_Disp_Goto_XY(SIGCH0_LINE_NUM,SIGNAL_DISP_COL_NUM);
	LCD_Disp_Str(signal_rep_disp);	
	LCD_Disp_Goto_XY(SIGCH0_LINE_NUM,PERCENT_DECPT_COL_NUM);
	LCD_Write_Data('.');
  LCD_Disp_Goto_XY(SIGCH0_LINE_NUM,PERCENT_DISP_COL_NUM);
	LCD_Write_Data('%');
	
	return;
	
}
	
/*------------------------------------------------------------*
FUNCTION NAME  : Delay_in_Micro_Sec

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        :   
-*------------------------------------------------------------*/
void Delay_in_Micro_Sec(const unsigned long num_micro_seconds)
{
	unsigned int i = 0;
	
	for (i = 0;  i < num_micro_seconds; ++i);
	return; 	
}

/*------------------------------------------------------------*
FUNCTION NAME  : Delay_in_Milli_Sec

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        :   
-*------------------------------------------------------------*/
void Delay_in_Milli_Sec(const unsigned long num_milli_seconds)
{
	unsigned int i = 0;
	
	for (i = 0;  i < num_milli_seconds; ++i)
	  Delay_in_Micro_Sec(100UL);
	return;	
}

/*------------------------------------------------------------*
FUNCTION NAME  : Delay_Time_By_Count

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        :    
-*------------------------------------------------------------*/
void Delay_Time_By_Count(long count_val)
{
	 while(count_val--);
	 return;
}

/*------------------------------------------------------------*-
FUNCTION NAME  : Write_Bit_in_Data

DESCRIPTION     : write bit valve to data's bit, without affecting other bits of data .
                . 0 is the least bit pos and 7 is the most bit pos 

INPUT          : 

OUTPUT         : none

NOTE           : 
-*------------------------------------------------------------*/
void Write_Bit_in_Data(unsigned int *const data_ptr, const unsigned int bit_pos, const unsigned int set_bit_val )
{
    /* unsigned int bit_pos_data_val = 0x01; // 00000001
	 
      // Left shift appropriate number of places, 	 so for bit_pos = 2, then bit_pos_data_val = 00000100 /
      bit_pos_data_val <<= bit_pos;
     // If we want 1 to be written at this bit_pos of data
     if (set_bit_val == 1)
     {
	    // set 1 to bit_pos and all other bits of data are unchanged 
        *data |= bit_pos_data_val; 
        return;
     }
      // If we want 0 bit value at bit_pos, then only that bit_pos of data is set to 0   and all other bits of data are unchanged. /
     bit_pos_data_val = ~bit_pos_data_val; // Complement
     // if bit_pos = 2, then  bit_pos_data_val= 11111011 /
	 // Bitwise AND 
     *data &= bit_pos_data_val; */
	 
	 if (set_bit_val == 1)
        Set_Bit_in_Data(data_ptr, bit_pos);
     else
       Clear_Bit_in_Data(data_ptr, bit_pos ); 
   return;
}
/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/	 
