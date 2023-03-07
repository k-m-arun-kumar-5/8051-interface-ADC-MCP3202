/**************************************************************************
   FILE          :    lcd.h
 
   PURPOSE       :    LCD Header
 
   AUTHOR        :    K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :  for  LCD disp 
 
  CHANGE LOGS    :
	   
 **************************************************************************/
 /* to avoid this header file to be included more than once, conditional directive is used  */
#ifndef _LCD_H
#define _LCD_H
                            
#define LCD_WRITE_ENABLE_PULSE_IN_USEC       (1)
#define LCD_ADDR_SETUP_IN_USEC               (1)
#define LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC (1)
#define LCD_DATA_SETUP_IN_USEC               (1)
#define LCD_WRITE_ENABLE_PULSE_DELAY_IN_USEC (1)
#define LCD_CLEAR_EXEC_IN_USEC             (1050UL)
#define LCD_CURSOR_RETURN_EXEC_IN_USEC     (1000UL)
#define LCD_OTHER_INST_EXEC_IN_USEC          (10)

#define INVALID_DATA               (0)
#define ALL_LINES                  (0)
#define NUM_LINE1                  (1)
#define NUM_LINE2                  (2)
#define NUM_LINE3                  (3)
#define NUM_LINE4                  (4)
#define NUM_COL1                   (1)

#define MAX_AVAIL_NUM_COLS                    (20)
#define MAX_AVAIL_NUM_LINES                   (4)    
#define MAX_AVAIL_NUM_CHARS_IN_LCD        (MAX_AVAIL_NUM_COLS * MAX_AVAIL_NUM_LINES) 

#define BEGIN_LOC_LINE_NUM_01                      (0x80)
#define BEGIN_LOC_LINE_NUM_02                      (0xC0)
// (0x94)
#define BEGIN_LOC_LINE_NUM_03                     (BEGIN_LOC_LINE_NUM_01 + MAX_AVAIL_NUM_COLS) 
//(0xD4) 
#define BEGIN_LOC_LINE_NUM_04                     (BEGIN_LOC_LINE_NUM_02  + MAX_AVAIL_NUM_COLS) 
// (0x93)
#define END_LOC_LINE_NUM_01                       (BEGIN_LOC_LINE_NUM_01 + MAX_AVAIL_NUM_COLS - 1)
//(0xD3)
#define END_LOC_LINE_NUM_02                       (BEGIN_LOC_LINE_NUM_02  + MAX_AVAIL_NUM_COLS - 1)
//(0xA7)
#define END_LOC_LINE_NUM_03                       (BEGIN_LOC_LINE_NUM_03  + MAX_AVAIL_NUM_COLS - 1) 
//(0xE7)
#define END_LOC_LINE_NUM_04                       (BEGIN_LOC_LINE_NUM_04 + MAX_AVAIL_NUM_COLS - 1)



#define CHECK_BUSY_FLAG             (1)
#define NO_CHECK_BUSY_FLAG          (0)

#define DATA_INTERFACE_8_BITS       (1) 
#define DATA_INTERFACE_4_BITS       (0)

#define CHAR_FONT_5_10_DOTS        (1)
#define CHAR_FONT_5_8_DOTS         (0)

#define MAX_DISP_NUM_LINES_2      (1)
#define MAX_DISP_NUM_LINES_1      (0)     



typedef struct {
		struct 	{
	    	unsigned int interface: 1;
		    unsigned int num_lines: 1;
		    unsigned int font: 1;
		    unsigned int check_bf: 1;
			unsigned int         : 4;
	    } conf;	    
   } lcd;

extern unsigned int lcd_avail_loc_within_limit;
extern unsigned int cur_disp_lcd_loc , cur_input_lcd_loc ;
extern unsigned int lcd_read_command;
extern lcd lcd_unit; 
extern disp_format_t disp_format_data; 

/* -------------------- public prototype declaration --------------------------------------- */
void LCD_Write_Pulse();
void LCD_Read_Pulse();
void LCD_Write_Command_NO_BF(const unsigned int lcd_cmd);
void LCD_Write_Command(const unsigned int lcd_cmd);
void LCD_Write_Data(const char lcd_disp_ch);
void LCD_Disp_Str(const char *lcd_disp_str);
void LCD_Disp_Num(const disp_format_t lcd_disp_data);
void LCD_Disp_Goto_XY(const unsigned int start_line_num, const unsigned int start_col_num);
void LCD_Init();
void LCD_Check_Busy();
unsigned int LCD_Read_Command();
void LCD_Clear_Screen();
void Write_Bit_in_Data(unsigned int *data_ptr, const unsigned int bit_pos, unsigned int set_bit_val );
void LCD_Const_Disp();

#endif

/*-------------------------------------------------------------------
  ------------------------ END OF FILE ------------------------------
-------------------------------------------------------------------*/
