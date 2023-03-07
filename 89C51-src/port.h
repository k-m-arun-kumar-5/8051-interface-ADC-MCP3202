/**************************************************************************
   FILE          :    port.h
 
   PURPOSE       :    port header - define port and its pin assignment.
 
   AUTHOR        :    K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :    port header - user interface to external device, such as LED, Switch,
 	connection to other microcontroller via RS232, USB, etc. 
 To avoid this header file to be included more than once, conditional directive is used  
	
  CHANGE LOGS     :  
	   
 **************************************************************************/
#ifndef _PORT_H
#define _PORT_H

// ------ see PC_O.C -----------------------------------------------
    
/* Pins 3.0(Rx) and 3.1 (Tx) are used by on chip UART of uC 8051. On Chip UART used for RS-232 interface.
NOTE: Port 0, Port2 and Port 3 has multiplexed function */

// Used for manually checking timing (in simulator) by using preload value for Timer 2. used for test purpose

/* -------  MCP3202 ADC Interface --------- */
sbit ADC_CS  = P1^4; 
sbit ADC_CLK = P1^5; 
sbit ADC_DO  = P1^6;
sbit ADC_DI  = P1^7; 

/* -------  LCD Interface --------- */
sbit LCD_RS  = P2^0;
sbit LCD_RW  = P2^1;
sbit LCD_EN  = P2^2;

#define LCD_DATA_PORT  (P2)

/* -------- LED Interface --------- */
sbit ERROR_LED        = P2^3;
sbit ADC_CONSTANT_LED = P1^0;

#endif 

/*-------------------------------------------------------------------
  ---- END OF FILE -------------------------------------------------
-------------------------------------------------------------------*/
