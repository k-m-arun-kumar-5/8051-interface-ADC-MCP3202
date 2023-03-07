/**************************************************************************
   FILE          :    io_conf.h
 
   PURPOSE       :   main peripherial configuration Header
 
   AUTHOR        :    K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
  
  CHANGE LOGS    :
	   
 **************************************************************************/
 
 #ifndef _IO_CONF_H
 #define _IO_CONF_H
 
 /* -------------------------------- debug conf -----------------------------------------*/
#define TRACE                                   (1U)
#define TRACE_ERROR                             (2U)

 /* -------------------------------- OSC Freq conf -------------------------------------*/
 /* set crystal Oscillator(stable operation) frequency (in Hz) here we set 11.0592MHz to get precise baud rate */
#define OSC_FREQ (11059200UL)

// Number of oscillations required to execute per instruction or increment a timer (12, etc)
// 12 – Original 8051 / 8052 and numerous modern versions
// 6 – Various Infineon and Philips devices, etc.
// 4 – Dallas 320, 520 etc.
// 1 – Dallas 420, etc.

#define OSC_PER_INST (12)

/* ------------------------------- LCD oper conf ---------------------------------------*/
 
 #define LCD_DISP_ERR_LINE_NUM            (NUM_LINE4) 
 
/* for 16 * 2 LCD disp */                             
#define BEGIN_LOC_LINE1                      (0X80)
#define BEGIN_LOC_LINE2                      (0xC0)
#define BEGIN_LOC_LINE3                      (0x94) 
#define BEGIN_LOC_LINE4                      (0xD4)
#define END_LOC_LINE1                        (0x8F)
#define END_LOC_LINE2                        (0xCF)
#define END_LOC_LINE3                        (0xA7) 
#define END_LOC_LINE4                        (0xE7)


#define CONFIGURE_MAX_NUM_LINES               (NUM_LINE4)
/* num cols = num of chars in a line */
#define CONFIGURE_MAX_NUM_COLS                (20)
#define CONFIGURE_MAX_NUM_CHARS_IN_LCD    (CONFIGURE_MAX_NUM_LINES * CONFIGURE_MAX_NUM_COLS ) 
#define MAX_NUM_CHARS_INPUT_DATA          (MAX_AVAIL_NUM_COLS) 

#define LCD_DATA_INTERFACE                DATA_INTERFACE_4_BITS  
#define LCD_BUSY_FLAG                     NO_CHECK_BUSY_FLAG
#define LCD_CHAR_FONT                     CHAR_FONT_5_8_DOTS  
#define LCD_MAX_DISP_NUM_LINES            MAX_DISP_NUM_LINES_2


/* ------------------------------- ADC oper conf ---------------------------------------*/ 

#define SIGCH0_LINE_NUM           (NUM_LINE2)
#define SIGNAL_DECPT_COL_NUM      (NUM_COL1 + 2)
#define SIGNAL_FRAC_COL_NUM       (SIGNAL_DECPT_COL_NUM + 1)
#define SIGNAL_DISP_COL_NUM       (SIGNAL_FRAC_COL_NUM + 1)    
#define PERCENT_INT_COL_NUM       (SIGNAL_DISP_COL_NUM + 4)
#define PERCENT_DECPT_COL_NUM     (PERCENT_INT_COL_NUM + 3)
#define PERCENT_FRAC_COL_NUM      (PERCENT_DECPT_COL_NUM + 1) 
#define PERCENT_DISP_COL_NUM      (PERCENT_FRAC_COL_NUM + 2) 
 
#define MAX_ANALOG_VALUE_CH0           (5U)
#define MIN_ANALOG_VALUE_CH0           (0U)
#define FULL_SCALE_ANALOG_VAL_CH0     (MAX_ANALOG_VALUE_CH0 - MIN_ANALOG_VALUE_CH0)

#define MAX_ADC_VALUE (4095UL)


#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
