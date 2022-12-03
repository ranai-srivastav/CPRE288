#include "lcd.h"
#include "open_interface.h"
#include "Timer.h"
#include <stdio.h>
#include "uart_extra_help.h"
//#include "Our_Functions.h"

extern volatile double dist_travelled; //distance since last scan

void move_forward(oi_t *sensor, int centimeters) {
    double sum = 0;
    oi_setWheels(50, 50); // move forward; full speed
    while (sum < centimeters*10) {
    oi_update(sensor);
    sum += sensor->distance;

    if(uart_flag)
            {
                uart_flag = 0;
                if((char)uart_data == 't')
                {
                    break;
                }
            }
    }
    oi_setWheels(0, 0); // stop
}

double move_backward(oi_t *sensor, int centimeters) {
    double sum = 0;
    oi_setWheels(-50, -50); // move forward; full speed
    while (abs(sum) < centimeters*10) {
    oi_update(sensor);
    sum += sensor->distance;

    if(uart_flag)
            {
                uart_flag = 0;
                if((char)uart_data == 't')
                {
                    break;
                }
            }
    }
    oi_setWheels(0, 0); // stop
    return sum;
}

void turn_clockwise(oi_t *sensor, float degrees) {
    double sum = 0;
    oi_setWheels(-50, 50); //Turn bot clockwise
    while (sum < abs(degrees)) {
        oi_update(sensor);
        sum += abs(sensor->angle);
        //lcd_clear();
        //int2char(sum);

        if(uart_flag)
                {
                    uart_flag = 0;
                    if((char)uart_data == 't')
                    {
                        break;
                    }
                }

    }
    oi_setWheels(0,0); //stop
}

void turn_counterclockwise(oi_t *sensor, float degrees) {
    double sum = 0;
    oi_setWheels(100, -100); //Turn bot clockwise
    while (sum < abs(degrees)) {
        oi_update(sensor);
        sum += abs(sensor->angle);

        if(uart_flag)
                {
                    uart_flag = 0;
                    if((char)uart_data == 't')
                    {
                        break;
                    }
                }

    }
    oi_setWheels(0,0); //stop
}

double move_until_bump(oi_t *sensor, int max_cm)
{
    double dist_travelled = 0;
    oi_setWheels(150, 150); // move forward; full speed

    while(!sensor->bumpLeft && !sensor->bumpRight && abs(dist_travelled) < max_cm)
    {
        oi_update(sensor);
        dist_travelled += sensor->distance/10.0;

        if(uart_flag)
        {
            uart_flag = 0;
            if((char)uart_data == 't')
            {
                break;
            }
        }

        if(sensor->bumpLeft)
            uart_sendChar('L');

        if(sensor->bumpRight)
            uart_sendChar('R');



    }
    oi_setWheels(0, 0);
    return dist_travelled;
}

bool hasBumpedLeft(oi_t *sensor)
{
    return sensor->bumpLeft;
}

bool hasBumpedRight(oi_t *sensor)
{
    return sensor->bumpRight;
}

int Perp2Tape(oi_t *sensor)
{
    oi_setWheels(50, 50);
    if((sensor->cliffLeftSignal > 2700) || (sensor->cliffRightSignal > 2700)) // Check for sensors detecting white tape, do nothing if neither detect anything
    {
        oi_update(sensor);
        //lcd_clear();
        //int2char(sensor->cliffLeftSignal);
        //lcd_putc(' ');
        //int2char(sensor->cliffRightSignal);

        oi_setWheels(0, 0);
        oi_update(sensor);

        while(sensor->cliffLeftSignal < 2700) // If left sensor hasn't detected the white tape
        {
            oi_setWheels(0, 25);
            oi_update(sensor);
        }

        while(sensor->cliffRightSignal < 2700) // If Right sensor hasn't detected the white tape
        {
            oi_setWheels(25, 0);
            oi_update(sensor);
        }

        if(sensor->cliffLeftSignal < 2700 || sensor->cliffRightSignal < 2700) // If the correction causes one sensor to come off the tape, recall the function and try again
        {
            oi_setWheels(-50, -50);
            oi_update(sensor);
            timer_waitMillis(1000);
            oi_setWheels(0, 0);
            Perp2Tape(sensor);

        }
        else // Bot is perpendicular to tape, will now begin process of checking the perimeter of area
        {
            move_backward(sensor, 30);
            turn_clockwise(sensor, 70);
            return 1;

        // Once bot is perpendicular to the tape, it will back up and turn 90 deg CW
        }
    }

    return 0; //If no tape is detected, ignore this call

}

void Perp2Hole(oi_t *sensor)
{
    oi_update(sensor);
    dist_travelled += sensor->distance/10.0;

    if((sensor->cliffLeftSignal < 10) || (sensor->cliffRightSignal < 10) || (sensor->cliffFrontRightSignal < 10) || (sensor->cliffFrontLeftSignal < 10))
    { // If statement checks for any of the sensors on the bot detecting a hole, this can be improved to have different reactions depending on sensor
        oi_setWheels(0, 0);
        oi_update(sensor);
        dist_travelled += sensor->distance/10.0; // Need to increment dist_travelled to ensure the bot stops to scan every 50 cm.

        /*
        while(sensor->cliffLeftSignal > 10)
        {
           oi_setWheels(0, 25);
           oi_update(sensor);
        }
        while(sensor->cliffRightSignal > 10)
        {
           oi_setWheels(25, 0);
           oi_update(sensor);
        }
        */

        move_backward(sensor, 30);
        turn_clockwise(sensor, 70);


    }
}
void TapeDrive(oi_t *sensor) // Function incomplete, should be similar to perp2edge in main, but with different scanning attributes.
{
    double dist_travelled = 0;
    int Boundary_Detected = 0;
    int max_cm = 50;
    //autostruct_t* min_vals;

    while(Boundary_Detected == 0)
    {
       oi_setWheels(50, 50);

       while(!sensor->bumpLeft && !sensor->bumpRight && abs(dist_travelled) < max_cm)
       {
           oi_update(sensor);
           dist_travelled += sensor->distance/10.0;
           Boundary_Detected = Perp2Tape(sensor); // chk for white tape
           Perp2Hole(sensor); //Chk 4 holes

           if(Boundary_Detected == 1){
               break;
           }
       }
       oi_setWheels(0, 0);

       if(sensor->bumpLeft || sensor->bumpRight)
       {
           move_backward(sensor, 10);
           turn_clockwise(sensor, 50);
           move_forward(sensor, 30);
           turn_counterclockwise(sensor, 65);
           move_forward(sensor, 15); //reverts input distance so total distance remains 200 cm
           oi_setWheels(0, 0);
       }
       if(abs(dist_travelled) >= max_cm) // Remove? Want to scan anyways, especially after bump
       {
           //min_vals = object_scan(sensor);
           // Condition for avoiding object that is directly ahead!!
           dist_travelled = 0;
           timer_waitMillis(1000);
           //Scan 0-110 degrees
       }
    }
}



