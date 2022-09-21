/**
 * lab4_template.c
 * 
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
#include <stdio.h>
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

#warning "Possible unimplemented functions"
#define REPLACEME 0



int main(void) {
	button_init();
	lcd_init();
	            // Don't forget to initialze the cyBot UART before trying to use it
	
	char line[10];
	int i=0;
	int j = 5;
	char ch = 'x';
	
	
	while(button_getButton() != 4)
	{
	
	    sprintf(line, "%d\0", button_getButton());
	    lcd_puts(line);

	    i++;
	}
	
	lcd_clear();


	return 0;
}
