/**
 * lab5_template.c
 * 
 * Template file for CprE 288 lab 5
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 *
 * @author Phillip Jones, updated 6/4/2019
 */

#include "button.h"
#include "timer.h"
#include "lcd.h"
#include <math.h>
#include "Timer.h"

#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

#include "cyBot_Scan.h"  // For scan sensors 


#warning "Possible unimplemented functions"
#define REPLACEME 0


// Defined in button.c : Used to communicate information between the
// the interupt handeler and main.
extern volatile int button_event;
extern volatile int button_num;

void send_string(char s[], int length)
{
    int  i = 0;
    for(i = 0; i<length; i++)
    {
        cyBot_sendByte(s[i]);
    }
}

/**
 *   y = 141.03e-0.002x
 *   y = 142.21e-0.002x
 *   y = 2E+07x-1.987
 *
 *
 */
float est_distance(int IR_sensor_val)
{
//    return 142.21*pow(M_E, -0.002*IR_sensor_val);
    return 2*pow(10, 7)*pow(IR_sensor_val, -1.987);
//    return
}

void perform_scan()
{
    cyBOT_Scan_t *val = malloc(sizeof(cyBOT_Scan_t));
    char line[38];

    int i = 0, sum = 0;
    for(i=0; i<10; i++)
    {
        cyBOT_Scan(90, val);
        sum += val->IR_raw_val;
    }

    int IR_sensor_val = sum/10;

    float cm = est_distance(IR_sensor_val);

    sprintf(line, "Raw: %04d,          Dist Est: %.2f", IR_sensor_val, cm);
    send_string(line, 38);

    lcd_puts(line);


//  Part 1
//  sprintf(line, "Raw: %d \r\n", val->IR_raw_val);
//  send_string(line, 12);

    free(val);
//    lcd_clear();
}


int main(void) {
	button_init();
	lcd_init();
	init_button_interrupts();

	            // Don't forget to initialze the cyBot UART before trying to use it


	cyBot_uart_init_clean();  // Clean UART initialization, before running your UART GPIO init code

	// Complete this code for configuring the  (GPIO) part of UART initialization
      SYSCTL_RCGCGPIO_R |= 0b000010;
      timer_waitMillis(1);            // Small delay before accessing device after turning on clock
      GPIO_PORTB_AFSEL_R |= 0b00000011;
      GPIO_PORTB_PCTL_R &= 0xFFFFFF00;     // Force 0's in the disired locations
      GPIO_PORTB_PCTL_R |= 0x00000011;     // Force 1's in the disired locations
      GPIO_PORTB_DEN_R |= 0b00000011;
      GPIO_PORTB_DIR_R &= 0b11111101;      // Force 0's in the disired locations
      GPIO_PORTB_DIR_R |= 0b00000001;      // Force 1's in the disired locataions

      cyBot_uart_init_last_half();  // Complete the UART device initialization part of configuration
	
	// Initialze scan sensors
     cyBOT_init_Scan(0b101);


//	char line[22];

    while(button_num != 4) //TODO make this cleanly exit
    {

        if(button_event == 1) //Global variable updated inside gpioe event handler
        {
            lcd_clear();
            perform_scan();
//            lcd_clear();
//            sprintf(line, "%d\0", button_num); //Displays rightmost button being pushed
//            lcd_puts(line);
            button_event = 0;
//            sprintf(line, "Button Pressed: %d\r\n\0", button_num);
//            send_string(line, 22); //Sends string to PuTTY
        }

    }
//

    return 0;
}
