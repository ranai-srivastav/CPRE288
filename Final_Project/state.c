/*
 * state.h
 *
 *  Created on: Dec 2, 2022
 *      Author: ranais
 */

#include "state.h"


void perp_to_edge(oi_t *sensor)
{
    int i;
    scan_data_t *Scandata;
    //double dist_travelled = 0;
    int Boundary_Detected = 0;
    int max_cm = 50;
    //autostruct_t* min_vals;

    while(Boundary_Detected == 0) // Boundary refers to the white tape boundary, 0 indicates it hasn't been detected
    {
        oi_setWheels(50, 50);

        while(!sensor->bumpLeft && !sensor->bumpRight && abs(dist_travelled) < max_cm)
        {
            oi_update(sensor);
            dist_travelled += sensor->distance/10.0;
            Boundary_Detected = Perp2Tape(sensor); // chk for white tape, will return 1 if it has reached the tape
            Perp2Hole(sensor); //Chk 4 holes, will avoid falling into hole if detected

            if(Boundary_Detected == 1){
                break;
            }
        }
        oi_setWheels(0, 0);

        if(sensor->bumpLeft || sensor->bumpRight) // TODO improve functionality
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

            //int angle;
            //min_vals = object_scan(sensor);
            // Condition for avoiding object that is directly ahead!!
            dist_travelled = 0;
            //timer_waitMillis(1000);
            Scandata = ScanVals(0,180);
            for( i = 0; i <= 90; i++){ // 90 Scans from 0-180 with a gap of 2 degrees between each, can be changed
                lcd_clear();
                int2char(Scandata[i].structIRdistance);
                uart_sendChar(' ');
                int2char(Scandata[i].structPingdistance);
                uart_sendChar('\n');
                uart_sendChar('\r');
                //Printed lines are to check for correctly populated structs
            }

            //Scan 0-180 degrees

        }
    }
    //TapeDrive();
    //perp_to_edge(sensor);


}
