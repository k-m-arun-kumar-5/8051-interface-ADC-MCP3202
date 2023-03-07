/* ********************************************************************
FILE                 : mcp3202.c

PURPOSE              : 
	 
AUTHOR               : K.M. Arun Kumar alias Arunkumar Murugeswaran
	 
KNOWN BUGS           : 

NOTE                 : MCU implementation supports only MSB First ADC Conversation. 									

CHANGE LOGS          :

*****************************************************************************/

#include "main.h"

// ------  data type declarations ---------------------------


// ------  function prototypes ------------------------------


// ------  constants ----------------------------------------



// ------  variables ----------------------------------------



/*------------------------------------------------------------*-
FUNCTION NAME  : MCP3202_Init

DESCRIPTION    : 							 

INPUT          : 

OUTPUT         : 

NOTE           : 
-*------------------------------------------------------------*/
void MCP3202_ADC_Init(void)
{
	 ADC_DO = 1; //ADC_DO is input pin for MCU
	 ADC_DI = 0; //ADC_DI is output pin for MCU
	 ADC_CLK = 0; //ADC_CLK is output pin for MCU
	 ADC_CS = 0; //ADC_CS is output pin for MCU
   return;
}

/*------------------------------------------------------------*-
FUNCTION NAME  : MCP3202_ADC_Read_Proc

DESCRIPTION    :  MCP3202 ADC module with SPI interface
	 
INPUT          :  

OUTPUT         :  

NOTE           : 
-*------------------------------------------------------------*/
unsigned long MCP3202_ADC_Read_Proc(const unsigned char adc_conf)
{
    unsigned char i,k = 0;
	unsigned int SPI_MCU_Tx, SPI_MCU_Rx, spi_mode; 
    unsigned long adc_result = 0 ;  //12 bit
	
	spi_mode = ((adc_conf) & (0x03 << ADC_SPI_MODE_BIT_POS)) >> ADC_SPI_MODE_BIT_POS;	
	/* #ifdef TRACE
	  printf("SPI mode : %u \r", spi_mode);
	#endif */
	
	switch(spi_mode)
	{
		case SPI_MODE_0:
		case SPI_MODE_1:
		   ADC_CLK = 0; 						// make clock low first, clock idle state is 0
		break;
        case SPI_MODE_2:
		case SPI_MODE_3:
           ADC_CLK = 1; 					   // make clock high first, clock idle state is 1 		  	 
		break;		
	}
	k++; k++;	
	ADC_CS = 0; 	// Active chip select
    k++;        	// Delay about 1 uS		
	SPI_MCU_Rx = 0x00; 
	SPI_MCU_Tx = 0x01; // LSB with Start Bit	
	for (i = 0; i < 8; ++i)
	{
		ADC_DI = Test_Bit_Is_Set_in_Data(&SPI_MCU_Tx, (7 - i));	// Transmit MSB first on clocks's falling edge with clock idle state 0 or 1					
		ADC_CLK = !ADC_CLK;	
		//ADC_CLK = 1; //
	    k++;k++;		
		SPI_MCU_Rx |= ((ADC_DO && 1) << (7 - i));  	// Receive MSB first on clocks's rising edge with clock idle state 0 or 1
		ADC_CLK = !ADC_CLK;
		//ADC_CLK = 0; 
	    k++;k++;		
	}
	SPI_MCU_Tx = 0x00; 	
	if(adc_conf & (1 << ADC_SGL_DIFF_MODE_BIT_POS)) 
	{
	   	SPI_MCU_Tx |= (1 << 7);
	}
	if(adc_conf & (1 << ADC_ODD_SIGN_MODE_BIT_POS)) 
	{
		SPI_MCU_Tx |= (1 << 6);
	}
	if(adc_conf & (1 << ADC_MSB_FIRST_MODE_BIT_POS)) 
	{
		SPI_MCU_Tx |= (1 << 5);
	}	
	SPI_MCU_Rx = 0x00;
    for (i = 0; i < 8; ++i)
	{
		ADC_DI = Test_Bit_Is_Set_in_Data(&SPI_MCU_Tx, (7 - i));	// Transmit MSB first on clocks's falling edge with clock idle state 0 or 1	
		ADC_CLK = !ADC_CLK;
		//ADC_CLK = 1; 						
	    k++;k++;	
		SPI_MCU_Rx |=  ((ADC_DO && 1) << (7 - i)); // Receive MSB first on clocks's rising edge with clock idle state 0 or 1
		ADC_CLK = !ADC_CLK;
		//ADC_CLK = 0; 
	    k++;k++;
	}	
	adc_result = ((SPI_MCU_Rx & 0x0F) << 8);
  //  adc_result[1] = SPI_MCU_Rx & 0x0F;
	SPI_MCU_Rx = 0x00;
	SPI_MCU_Tx = 0x00; 	
	for (i = 0; i < 8; ++i)
	{
		ADC_DI = Test_Bit_Is_Set_in_Data(&SPI_MCU_Tx, (7 - i));	// Transmit MSB first on clocks's falling edge with clock idle state 0 or 1	
		ADC_CLK = !ADC_CLK;
		//ADC_CLK = 1; 						
	    k++;k++;	
		SPI_MCU_Rx |= ((ADC_DO && 1) << (7 - i)); // Receive MSB first on clocks's rising edge with clock idle state 0 or 1
		ADC_CLK = !ADC_CLK;
		//ADC_CLK = 0; 
	    k++;k++;
	}
	ADC_CS = 1; 	
	adc_result = (adc_result & 0xFF00L) | (SPI_MCU_Rx & 0x00FFL);
	//adc_result[0] = SPI_MCU_Rx; 
	return adc_result;
}

/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void Encoded_To_Actual_Analog_Val_Calc(const unsigned long adc_value, unsigned long int full_scale_analog_val, \
   unsigned long int min_analog_val, unsigned long *const analog_val_in_digital_int, unsigned long *const analog_val_in_digital_frac )
{
	unsigned long remainder_val;	
	*analog_val_in_digital_int = ((full_scale_analog_val * adc_value) /  (MAX_ADC_VALUE)) + min_analog_val;
	remainder_val = (full_scale_analog_val * adc_value) %  MAX_ADC_VALUE ;
	*analog_val_in_digital_frac = (remainder_val * 10) / MAX_ADC_VALUE;
}
/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :
								
INPUT          : none

OUTPUT         : 

NOTE           : 

Func ID        : 21   
-*------------------------------------------------------------*/
void Encoded_To_Percent_Calc(const unsigned long adc_value, unsigned int *const percent_int, unsigned int *const percent_frac )
{
	unsigned int remainder_val;
	unsigned long temp_percent_int;
	temp_percent_int =  (100 * adc_value); 
	*percent_int = (100 * adc_value) /  MAX_ADC_VALUE;
	remainder_val = temp_percent_int %  MAX_ADC_VALUE;
	*percent_frac = (remainder_val * 10) / MAX_ADC_VALUE;
}

/* -------------------------------------------------------------------
  ---- END OF FILE -------------------------------------------------
------------------------------------------------------------------- */
