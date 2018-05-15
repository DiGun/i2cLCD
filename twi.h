﻿#ifndef TWI_H_
#define TWI_H_
#include <avr/interrupt.h>
#include <util/delay.h>
//#include "main.h"

void I2C_Init (void); //инициализация i2c
void I2C_StartCondition(void); //Отправим условие START
void I2C_StopCondition(void); //Отправим условие STOP
void I2C_SendByte(unsigned char c); //передача байта в шину
void I2C_SendByteByADDR(unsigned char c,unsigned char addr); //передача байта в шину на устройство по адресу
unsigned char I2C_ReadByte(void); //читаем байт
unsigned char I2C_ReadLastByte(void); //читаем последний байт


#endif /* TWI_H_ */