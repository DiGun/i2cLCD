/*
 * LCD_I2C.h
 *
 * Created: 13.05.2018 18:51:37
 *  Author: digun
 */ 


#ifndef LCD_I2C_H_
#define LCD_I2C_H_
#include <stdio.h>
#include "twi.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0B00000100  // Enable bit
#define Rw 0B00000010  // Read/Write bit
#define Rs 0B00000001  // Register select bit

void LCD_init(void);
void expanderWrite(uint8_t _data);
void write4bits(uint8_t value);
void pulseEnable(uint8_t _data);
void command(uint8_t value);
void send(uint8_t value, uint8_t mode);

void home();
void clear();
void createChar(uint8_t, uint8_t[]);

void display();
void noDisplay();

void noCursor();
void cursor();
void setCursor(uint8_t col, uint8_t row);

void noBlink();
void blink();
  
void backlight(void);
void noBacklight(void);
void setBacklight(uint8_t new_val);

void createChar(uint8_t location, uint8_t charmap[]);
void send_char(char c);

static int fputchar(char c, FILE *stream)
{
	send((uint8_t)c, Rs);
	return 0;
}


#endif /* LCD_I2C_H_ */