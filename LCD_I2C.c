/*
 * CFile1.c
 *
 * Created: 13.05.2018 18:51:57
 *  Author: digun
 */ 

#include "LCD_I2C.h"

#define _cols 16
#define _rows 2
#define _numlines 2
#define _Addr (0x27<<1)

  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;
  uint8_t _backlightval;

#define printIIC(args)	I2C_SendByteByADDR(args,_Addr)

void expanderWrite(uint8_t _data)
{
//	I2C_StartCondition();
	I2C_SendByteByADDR((_data) | _backlightval,_Addr);
//	printIIC((_data) | _backlightval);
//	I2C_StopCondition();
}

void write4bits(uint8_t value) 
{
	expanderWrite(value);
	pulseEnable(value);
}

void pulseEnable(uint8_t _data)
{
	expanderWrite(_data | En);	// En high
	_delay_ms(1);		// enable pulse must be >450ns
	
	expanderWrite(_data & ~En);	// En low
	_delay_ms(50);		// commands need > 37us to settle
}


/*********** mid level commands, for sending data/cmds */

inline void command(uint8_t value) 
{
	send(value, 0);
}


/************ low level data pushing commands **********/

// write either command or data
void send(uint8_t value, uint8_t mode) 
{
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
	write4bits((highnib)|mode);
	write4bits((lownib)|mode);
}

// Turn the display on/off (quickly)
void display() 
{
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void noDisplay() 
{
	_displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void home()
{
	command(LCD_RETURNHOME);  // set cursor position to zero
	_delay_ms(1000);  // this command takes a long time!
}

/********** high level commands, for the user! */
void clear()
{
	command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	_delay_ms(1000);  // this command takes a long time!
}


// Turn the (optional) backlight off/on
void noBacklight(void)
{
	_backlightval=LCD_NOBACKLIGHT;
	expanderWrite(0);
}

void backlight(void)
{
	_backlightval=LCD_BACKLIGHT;
	expanderWrite(0);
}

void setBacklight(uint8_t new_val)
{
	if(new_val){
		backlight();		// turn backlight on
	}else{
		noBacklight();		// turn backlight off
	}
}

void setCursor(uint8_t col, uint8_t row)
{
int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
if ( row > _numlines ) {
	row = _numlines-1;    // we count rows starting w/0
}
command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void send_char(char c)
{
	send((uint8_t)c, Rs);	
}



void LCD_init()
{
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	_backlightval = LCD_NOBACKLIGHT;
	I2C_Init ();
	
//	if (lines > 1) {
		_displayfunction |= LCD_2LINE;
//	}
//	_numlines = lines;

	// for some 1 line displays you can select a 10 pixel high font
/*	
	if ((dotsize != 0) && (lines == 1)) {
		_displayfunction |= LCD_5x10DOTS;
	}
*/
	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	_delay_ms(50);
	
	// Now we pull both RS and R/W low to begin commands
	expanderWrite(_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
	_delay_ms(500);

	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46


	// we start in 8bit mode, try to set 4 bit mode
	write4bits(0x03 << 4);
	_delay_us(4500); // wait min 4.1ms
	
	// second try
	write4bits(0x03 << 4);
	_delay_us(4500); // wait min 4.1ms

	// third go!
	write4bits(0x03 << 4);
	_delay_us(150);
	
	// finally, set to 4-bit interface
	write4bits(0x02 << 4);


	// set # lines, font size, etc.
	command(LCD_FUNCTIONSET | _displayfunction);
	
	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	display();
	
	// clear it off
	clear();
	
	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	
	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);
	
	home();
}