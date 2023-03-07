/* ********************************************************************
FILE                 : uart.c

PURPOSE              : 
	 
AUTHOR               : K.M. Arun Kumar alias Arunkumar Murugeswaran
	 
KNOWN BUGS           : 

NOTE                 : 								

CHANGE LOGS          :

*****************************************************************************/

#include "main.h"

// ------  data type declarations ---------------------------


// ------  function prototypes ------------------------------


// ------  constants ----------------------------------------



// ------  variables ----------------------------------------



/*------------------------------------------------------------*-
FUNCTION NAME  : Serial_Init

DESCRIPTION    : Initialize serial port						 

INPUT          : 

OUTPUT         : 

NOTE           : 
-*------------------------------------------------------------*/
void Serial_Init(void)
{
  	SCON = 0x52;    // setup serial port control 
  	TMOD = 0x20;    // hardware (9600 BAUD @11.05592MHZ) 
  	TH1  = 0xFD;    // TH1
  	TR1	 = 1;  		  // Timer 1 on
}

/*-------------------------------------------------------------------
  ------------------------ END OF FILE ------------------------------
-------------------------------------------------------------------*/

