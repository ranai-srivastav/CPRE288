/*
 * Our_Functions.c
 *
 *  Created on: Oct 3, 2022
 *      Author: nayers99
 */
#include "Our_Functions.h"
//#include "cyBot_uart.h"
#include <math.h>
#include "lcd.h"
//#include "ADC.h"
#include "open_interface.h"
//#include "movement.h"


void int2char(int input){
        int i = 0;
        char savearray[4] = {0};
        char modarray[4] = {0};
        int hold = input;
        int count = 0;

        if(input == 0){
            count = 1;
        }
        while(hold > 0){ //this loop is used to count the # of decimal places
            hold /= 10;
            count ++;
        }
        hold = input;
        for(i = 0; i < count; i++){
            savearray[i] = (hold % 10) + '0'; //sets savearray[i] equal to char representation of most sig. digit of input
            hold /= 10; //shifts decimal place of input
         }
        for (i = 0; i < count; i++) {
            modarray[i] = savearray[count - i - 1]; //reverses the string, correcting the order of digits
        }

        for(i = 0; i < count; i++){
            //cyBot_sendByte(modarray[i]); if sending to Putty
            //lcd_putc(modarray[i]); // if sending to cyBot
            uart_sendChar(modarray[i]); //fuck you
         }
}

int Raw2dist(int IR_val){
    double distance = 0;
    int integerdist = 0;
    //distance = (4 * pow(10, 6)) * (pow(IR_val, -1.618)); // y = 4E+06x^-1.615
    distance = 234406 * (pow(IR_val, -1.259)); // y = 234406x^-1.26
    //distance = (-8 * pow(10, -9) * pow(IR_val, 3)) + (5 * pow(10, -5) * pow(IR_val, 2)) + (-.1336 * IR_val) + 126.38;
    integerdist = round(distance);

    return integerdist;
    // This function generates a distance converted from the IR value, using a function created by our testing.

}

/*
int averagedIRdist(){
    int i = 0, sum = 0, distance;

    for(i = 0; i < 10; i++){ // avg of 30 scans allows us to get a more accurate output.
        sum += get_rawIRval();
        timer_waitMillis(35);
    }
    distance = Raw2dist(sum / 10);
    return distance;
}
*/




