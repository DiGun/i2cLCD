/*
 * i2cLCD.c
 *
 * Created: 13.05.2018 12:41:46
 * Author : digun
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "LCD_I2C.h"


int main(void)
{
	static FILE mystdout = FDEV_SETUP_STREAM(fputchar, NULL, _FDEV_SETUP_WRITE);
	stdout=&mystdout;
//	DDRD=1<<PORTD4;

	LCD_init();
	backlight();
	
	printf("chuguev.net");
	setCursor(8,1);
	printf("LCD 1602");

	uint8_t n=0;
	while(1)
	{
//		PORTD^=1<<PORTD4;
		setCursor(0,1);
		printf("%3d",n++);
		_delay_ms(1000);
	}
}