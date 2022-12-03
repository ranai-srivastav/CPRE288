//#include "cyBot_uart.h"
#include "lcd.h"
#include "string.h"
#include "open_interface.h"
#include "movement.h"
#include "cyBot_Scan.h"
#include "timer.h"
#include "math.h"
#include "Our_functions.h"
#include "uart.h"
#include "ADC.h"


typedef struct{
    int structangle;
    int structIRdistance;
    int structPingdistance;
    //Scanned data is assigned to a variable of this struct, there will 91 instaqnces to account for all scans.
}scan_data;

typedef struct{
    int startAngle;
    int endAngle;
    int linWidth;
    int distAtObject;
    //calculated data for seen objects will be stored in here.
}obj;


extern volatile int interruptevent;
extern volatile char interruptchar;
extern volatile int bumpevent;
volatile char inputChar;

void main(){


    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    cyBOT_init_Scan(0b0111);

    ADC0_init();
    uart_init();

    cyBOT_init_Scan(0b0111);
    right_calibration_value = 274750; //Cybot 00
    left_calibration_value = 1251250; //Cybot 00
    //right_calibration_value = 264250; //Cybot 03
    //left_calibration_value = 1219750; //Cybot 03
    //right_calibration_value = 290500; //Cybot 06
    //left_calibration_value = 1235500; //Cybot 06
    //right_calibration_value = 248500; //Cybot 06
    //left_calibration_value = 1261750; //Cybot 06
    lcd_init();
    cyBOT_Scan_t Scanner;
    //cyBOT_SERVO_cal();

/*
    while(1){
        if(interruptevent == 1){
            char recieve = uart_receive();
            //dynamicmove(recieve);
            uart_sendChar(recieve);
            interruptevent = 0;
        }
    }

*/




    scan_data dataarray[91];
    char banner[] = "Angle PingDist IRDist";
    char banner2[] = "Object# Angle Distance Width";
    char banner3[] = "Target Angle   Current Angle   Distance to Go";//hard coded headers for putty output columns
    int angle, i = 0, on = 0;
    int IRdistance = 0, sum = 0, j, IRscans = 3;
    obj objects[10];

    int distance2go = 0;
    int robotDistance = 0;
    int angle2turn = 0;
    float distanceGone = 0;
    float currentAngle = 90;
    double trigAngle = 0;


   // cyBOT_init_Scan(0b0111);
    cyBOT_Scan(90, &Scanner);

    while(1) {
        if(interruptevent == 1){
            interruptevent = 0;
            inputChar = uart_receive();
            dynamicmove(inputChar);
        }

        //int2char(get_rawIRval());
        //uart_sendChar('\r');


    //oi_free(sensor_data);

    //if a putty input is detected, the input is stored in inputChar and fed to dynamicmove
        oi_update(sensor_data);
        currentAngle += (int)sensor_data->angle; //increments current angle based on updated delta from sensor
        distanceGone += (sensor_data->distance / 10);//increments current distance based on updated delta from sensor

        if (sensor_data->bumpLeft){ // if left bumper contact is detected, alert the user!!
            int i = 0;
            char bumpleft[] = "Bump left";
            for(i = 0; i < strlen(bumpleft); i++){
                cyBot_sendByte(bumpleft[i]);
            }
            cyBot_sendByte('\n');
            cyBot_sendByte('\r');
        }
        if (sensor_data->bumpRight){ // if right bumper contact is detected, alert the user!!
            int i = 0;
            char bumpRight[] = "Bump Right";
            for(i = 0; i < strlen(bumpRight); i++){
                cyBot_sendByte(bumpRight[i]);
            }
            cyBot_sendByte('\n');
            cyBot_sendByte('\r');
        }


        if(inputChar == 'm') { // Begins Scan
            inputChar = 0;

            for(i = 0; i < strlen(banner); i++){
                    cyBot_sendByte(banner[i]);
            }
            cyBot_sendByte('\n');
            cyBot_sendByte('\r');

            i = 0;

            for(angle = 0; angle <= 180; angle += 2){
                //for(j = 0; j < IRscans; j++){
                    //cyBOT_Scan(angle, &Scanner);
                    //sum +=
                    //sum += Scanner.IR_raw_val;
                //}
                cyBOT_Scan(angle, &Scanner);
                IRdistance = averagedIRdist();
                dataarray[i].structIRdistance = IRdistance;
                dataarray[i].structPingdistance = Scanner.sound_dist;
                dataarray[i].structangle = angle;
                //These two lines above send data from the scan straight to the struct, allowing us to delay the print...
                // ... to putty until 'm' is pressed.
                int2char(dataarray[i].structangle);
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                int2char(dataarray[i].structPingdistance);
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                int2char(dataarray[i].structIRdistance);
                cyBot_sendByte('\n');
                cyBot_sendByte('\r');
                i++;
                sum = 0;
            }



            int objectnum = 0;
            for(i = 5; i <= 90; i++){
                if(dataarray[i].structIRdistance <= 80 && on == 0){
                    objects[objectnum].startAngle = dataarray[i].structangle;
                    objects[objectnum].distAtObject = dataarray[i].structIRdistance;
                    on = 1;
                }
                else if(dataarray[i].structIRdistance >= 80 && on == 1){
                    objects[objectnum].endAngle = dataarray[i].structangle;
                    on = 0;
                    objectnum ++;
                }
                // This for loop checks for object conditions and will turn on when an object is closer than 50 cm...
                // ... it will turn off when those conditions are no longer satisfied, and assign the objects to objects struct
            }


           // while(cyBot_getByte() != 'm'){

            //}

            for(i = 0; i < strlen(banner2); i++){
                    cyBot_sendByte(banner2[i]);
            }
            cyBot_sendByte('\n');
            cyBot_sendByte('\r');

            for(i = 0; i < objectnum; i++){
                //objects[i].linWidth = (2 * objects[i].distAtObject) * sin(objects[i].startAngle / 2);
                int2char(i);
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                int2char(objects[i].startAngle);
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                int2char(objects[i].distAtObject);
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                cyBot_sendByte(' ');
                int2char(abs(objects[i].startAngle - objects[i].endAngle));
                cyBot_sendByte('\n');
                cyBot_sendByte('\r');
            }

            int smallestWidth = 1000;
            int object2Point;

            for(i = 0; i < objectnum; i++){
                if(abs(objects[i].startAngle - objects[i].endAngle) < smallestWidth && abs(objects[i].startAngle - objects[i].endAngle) > 6){
                    smallestWidth = abs(objects[i].startAngle - objects[i].endAngle);
                    object2Point = i;
                }
            }

            int turnangle = (objects[object2Point].startAngle + objects[object2Point].endAngle) / 2;
            // sets the angle that the scanner will rotate to.
            cyBOT_Scan(turnangle, &Scanner);
            timer_waitMillis(300);


            //distance2go = objects[object2Point].distAtObject;

            distance2go = Scanner.sound_dist;
            trigAngle = (180.0 / M_PI) * atan(
                    (10.0 + ((distance2go + 5.0) * (sin(turnangle * (M_PI / 180.0)))))
                    /
                    (((distance2go + 5.0) * (cos(turnangle * (M_PI / 180.0))))));

            robotDistance =
                    (10.0 + ((distance2go + 5.0) * (sin(turnangle * (M_PI / 180.0)))))
                    /
                    (sin(trigAngle * (M_PI / 180.0)));
            robotDistance -= 18;
           // printf("%d", robotDistance);

            if(trigAngle <= 0){
                trigAngle += 180;
            }

            angle2turn = turnangle; //gives us a target angle corresponding to the center of the smallest width object
            oi_update(sensor_data);
            currentAngle = 90;
            distanceGone = 0;

            distance2go -= 10;
            //previous 3 lines reposition the bot to "0,0" and update sensor for future deltas

            if(bumpevent == 1){
                inputChar = 'r';
                bumpevent = 0;
            }
      }

      if(inputChar == ' '){ //movement is stopped with spacebar, it also prints out data related to distance and angle

            inputChar = 0;
            //oi_update(sensor_data);
            //currentAngle += (int)sensor_data->angle; //increments current angle based on updated delta from sensor
            //distanceGone += (sensor_data->distance / 10);//increments current distance based on updated delta from sensor

            //every time you stop moving, the sensor_data is updated and the position of the bot is adjusted accordingly

            for(i = 0; i < strlen(banner3); i++){
                cyBot_sendByte(banner3[i]);
            }

            cyBot_sendByte('\n');
            cyBot_sendByte('\r');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            //int2char(distance2go);
            cyBot_sendByte(' ');
            int2char(trigAngle);
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            int2char(currentAngle);
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            cyBot_sendByte(' ');
            int2char(distance2go - (int)distanceGone);
            cyBot_sendByte('\n');
            cyBot_sendByte('\r');
        }
      if(inputChar == 'r'){ // function for autonomous movement, it utilizes "recursion" if a bump is detected to rescan.
          if(trigAngle <= 90){
              turn_clockwise(sensor_data, -1 * (90 - trigAngle));
          }
          else {
              turn_counter_clockwise(sensor_data, trigAngle - 90);
          }
          if(inputChar != ' '){
              move_forward(sensor_data, distance2go);
          }
          //oi_update(sensor_data);
          if(bumpevent == 1){
              inputChar = 'm';
          }
          else{
              inputChar = 0;
          }
      }


      if(inputChar == 'x') { // exit condition and clear the sensor data.
          inputChar = 0;
          oi_free(sensor_data);
          break;
      }
      //exit condition, frees sensor_data
}


}
