#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"
#include "button.h"
#include "movement.h"
#include "Our_Functions.h"
#include "ping.h"
#include "servo.h"
#include "adc.h"
#include "uart_extra_help.h"
//#include "Lab10_movement.h"

volatile double dist_travelled = 0;

enum exit_condition
{
    HOLE_FRONT,
    HOLE_LEFT,
    HOLE_RIGHT,
    EDGE_FRONT,
    EDGE_LEFT,
    EDGE_RIGHT,
    BUMP_LEFT,
    BUMP_RIGHT
};

void perp_to_edge(oi_t *sensor)
{
    int i;
    data *Scandata;
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


int main(void)
{
    lcd_init();
    ping_init();
    servo_init();
    adc_init();
    timer_init();
    uart_init(115200);

    oi_t *sensor = oi_alloc();
    oi_init(sensor);
    button_init();
    servo_rotate(90);

    char IR_vals[80];
    int i = 0;
    for(i = 0; i<80; i++)
    {
        IR_vals[i] = 0;
    }

//    while(!button_getButton())
//    {
////        lcd_clear();
//        sprintf(IR_vals, "FL-IR:   %10d FR-IR:   %10d Lf-IR:   %10d Rt-IR:   %10d ", sensor->cliffFrontLeftSignal, sensor->cliffFrontRightSignal, sensor->cliffLeftSignal, sensor->cliffRightSignal);
//        lcd_puts(IR_vals);
//        oi_update(sensor);
//
//        timer_waitMillis(3000);
//    }

    perp_to_edge(sensor);

    oi_free(sensor);

	return 0;
}

//exit_condition move_until_exit(oi_t *sensor)
//{
//
//}
