/**
 * lab6_template_extra_help.c
 *
 * Description: This is file is meant for those that would like a little
 *              extra help with formatting their code.
 *
 */

#define _RESET 0
#define _PART1 0
#define _PART2 0
#define _PART3 1

#include "timer.h"
#include "lcd.h"
//#include "uart.h"
#include "uart_extra_help.h"

#if _RESET
#include "Simulation_User Files_resetSimulation.h"
#endif

// Adding global volatile varibles for communcating between 
// your Interupt Service Routine, and your non-interupt code.

//volatile  char uart_data;  // Your UART interupt code can place read data here
volatile  char flag;       // Your UART interupt can update this flag
                           // to indicate that it has placed new data
                           // in uart_data                     


void main()
{
#if _RESET
    resetSimulationBoard();
#else

    timer_init();
    lcd_init();
    uart_init(115200);
    uart_interrupt_init();

#if _PART1      // Receive and display text
    int buffer = 0;
    char data;
    char line[21] = "";
    char s[1];
    while(1)
    {
        int i = 0;
        for(i = 0 ; i<21; i++)
        {
            line[i] = 0;
        }

        while (buffer < 20 && data != '\x0d')
        {
            data = uart_receive();
            if(data != '\x0d')
            {
                sprintf(s, "%c" ,data);
                strcat(line, s);
            }
            buffer++;
        }
        lcd_clear();
//        strcat(line, "\0");
        lcd_puts(line);
        buffer = 0;
        data = '\0';
    }
	

#endif

#if _PART2      // Echo Received Character
    int buffer = 0;
        char data;
        char line[21] = "";
        char s[1];
        while(1)
        {
            int i = 0;
            for(i = 0 ; i<21; i++)
            {
                line[i] = 0;
            }

            while (buffer < 20 && data != '\x0d')
            {
                data = uart_receive();
                uart_sendChar(data);
                if(data != '\x0d')
                {
                    sprintf(s, "%c" ,data);
                    strcat(line, s);
                }
                buffer++;
            }
            lcd_clear();
    //        strcat(line, "\0");
            lcd_puts(line);
            uart_sendChar('\n');
            uart_sendChar('\r');
            buffer = 0;
            data = '\0';
        }


#endif

#if _PART3 // Receive and send characters using interrupts.
        flag = 0;
        while (1) {

            if (flag == 1) {
                uart_sendChar(uart_data);
                lcd_putc(uart_data);
                flag = 0;
            }
        }
 
#endif

#endif
}

