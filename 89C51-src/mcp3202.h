/* ********************************************************************
FILE                 : mcp3202.h

PURPOSE              :  type declaration for SPI interface to ADC module (MCP3202)
	 
AUTHOR               : K.M. Arun Kumar alias Arunkumar Murugeswaran
	 
KNOWN BUGS           : 

NOTE                 : see mcp3202.c for details 

CHANGE LOGS          :

*****************************************************************************/	
#ifndef _MCP3202_H
#define _MCP3202_H

// ------ Public function prototypes -------------------------------

#define ADC_MSB_FIRST_MODE_BIT_POS  (0)
#define ADC_ODD_SIGN_MODE_BIT_POS   (1) 
#define ADC_SGL_DIFF_MODE_BIT_POS   (2)
#define ADC_SPI_MODE_BIT_POS        (3)  

              
// -------- public data constant definition -----------------

/* system ticks interval in m sec. for Every TICK_INTERVAL, application Task followed by e0S(system Task)
   is executed. NOTE: make sure that SYSTEM_TICK_INTERVAL is large enough that uC executing maximum time 
	 required to start and completes executing Application task and system task(sEOS_Go_To_Sleep) 	*/
	 
	typedef enum {ADC_LSB_FIRST = 0,  ADC_MSB_FIRST, ADC_DIFF_MODE = 0, 
              ADC_SGL_MODE, ADC_CH_00 = 0, ADC_CH_01} mcp3202_mode_t;   

   typedef enum {SPI_MODE_0 , SPI_MODE_1, SPI_MODE_2, SPI_MODE_3  } spi_modes_t;
   
void MCP3202_ADC_Init(void);
unsigned long MCP3202_ADC_Read_Proc(const unsigned char adc_conf);
void Encoded_To_Actual_Analog_Val_Calc(const unsigned long adc_value, unsigned long int full_scale_analog_val, \
   unsigned long int min_analog_val, unsigned long *const analog_val_in_digital_int, unsigned long *const analog_val_in_digital_frac );
void Encoded_To_Percent_Calc(const unsigned long adc_value, unsigned int *const percent_int, unsigned int *const percent_frac );
  
#endif

/*-------------------------------------------------------------------
  ---- END OF FILE -------------------------------------------------
-------------------------------------------------------------------*/
