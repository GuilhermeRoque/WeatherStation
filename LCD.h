/* 
 * File:   LCD.h
 * Author: marcone
 *
 * Created on 27 de Novembro de 2019, 09:47
 */

#ifndef LCD_H_
#define LCD_H_

#include "GPIO.h"

class LCD {
public:

	LCD(uint8_t rs,uint8_t rw, uint8_t en, uint8_t port);
	void LCD_Command(unsigned char cmnd);
	void LCD_Char (unsigned char char_data);
	void LCD_Init (void);
	void LCD_String (char *str);
	void LCD_Clear();

private:
    GPIO RS, RW, EN;
    GPIO_PORT::GPIO_Port * _Px;
};

#endif /* LCD_H_ */

