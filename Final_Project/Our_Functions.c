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

//void random()
//{
//    int objectnum = 0;
//                for(i = 5; i <= 90; i++){
//                    if(dataarray[i].structIRdistance <= 80 && on == 0){
//                        objects[objectnum].startAngle = dataarray[i].structangle;
//                        objects[objectnum].distAtObject = dataarray[i].structIRdistance;
//                        on = 1;
//                    }
//                    else if(dataarray[i].structIRdistance >= 80 && on == 1){
//                        objects[objectnum].endAngle = dataarray[i].structangle;
//                        on = 0;
//                        objectnum ++;
//                    }
//                    // This for loop checks for object conditions and will turn on when an object is closer than 50 cm...
//                    // ... it will turn off when those conditions are no longer satisfied, and assign the objects to objects struct
//                }
//
//
//               // while(cyBot_getByte() != 'm'){
//
//                //}
//
//                for(i = 0; i < strlen(banner2); i++){
//                        cyBot_sendByte(banner2[i]);
//                }
//                cyBot_sendByte('\n');
//                cyBot_sendByte('\r');
//
//                for(i = 0; i < objectnum; i++){
//                    //objects[i].linWidth = (2 * objects[i].distAtObject) * sin(objects[i].startAngle / 2);
//                    int2char(i);
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    int2char(objects[i].startAngle);
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    int2char(objects[i].distAtObject);
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    cyBot_sendByte(' ');
//                    int2char(abs(objects[i].startAngle - objects[i].endAngle));
//                    cyBot_sendByte('\n');
//                    cyBot_sendByte('\r');
//                }
//
//                int smallestWidth = 1000;
//                int object2Point;
//
//                for(i = 0; i < objectnum; i++){
//                    if(abs(objects[i].startAngle - objects[i].endAngle) < smallestWidth && abs(objects[i].startAngle - objects[i].endAngle) > 6){
//                        smallestWidth = abs(objects[i].startAngle - objects[i].endAngle);
//                        object2Point = i;
//                    }
//                }
//
//                int turnangle = (objects[object2Point].startAngle + objects[object2Point].endAngle) / 2;
//                // sets the angle that the scanner will rotate to.
//                cyBOT_Scan(turnangle, &Scanner);
//                timer_waitMillis(300);
//
//
//                //distance2go = objects[object2Point].distAtObject;
//
//                distance2go = Scanner.sound_dist;
//                trigAngle = (180.0 / M_PI) * atan(
//                        (10.0 + ((distance2go + 5.0) * (sin(turnangle * (M_PI / 180.0)))))
//                        /
//                        (((distance2go + 5.0) * (cos(turnangle * (M_PI / 180.0))))));
//
//                robotDistance =
//                        (10.0 + ((distance2go + 5.0) * (sin(turnangle * (M_PI / 180.0)))))
//                        /
//                        (sin(trigAngle * (M_PI / 180.0)));
//                robotDistance -= 18;
//               // printf("%d", robotDistance);
//
//                if(trigAngle <= 0){
//                    trigAngle += 180;
//                }
//
//                angle2turn = turnangle; //gives us a target angle corresponding to the center of the smallest width object
//                oi_update(sensor_data);
//                currentAngle = 90;
//                distanceGone = 0;
//
//                distance2go -= 10;
//                //previous 3 lines reposition the bot to "0,0" and update sensor for future deltas
//
//                if(bumpevent == 1){
//                    inputChar = 'r';
//                    bumpevent = 0;
//                }
//          }
//}

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




