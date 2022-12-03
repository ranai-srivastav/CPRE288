#include <calibration.h>

volatile short cc_RWP = 50;
volatile short cc_LWP = -50;

volatile short c_RWP = -50;
volatile short c_LWP = 50;

volatile short forward_RWP = 50;
volatile short forward_LWP = 50;

float degree_accuracy = 1;
float mm_accuracy = 1;

void cal_turn_counter_clockwise(oi_t *sensor, float input_angle)
{

    double mm_traveled = 0;
    double current_angle = 0;

    while(current_angle < (input_angle - 0.5))
    {
        oi_update(sensor);

        mm_traveled += sensor->distance;
        current_angle += sensor->angle;

        if(mm_traveled > mm_accuracy)
        {
            cc_RWP -= 1;
            mm_traveled = 0;
        }

        if(mm_traveled < -(mm_accuracy))
        {
            cc_RWP += 1;
            mm_traveled = 0;
        }

        oi_setWheels(cc_RWP, cc_LWP);
        //lcd_printf("LWP= %d RWP= %d     %f", cc_LWP, cc_RWP, current_angle);
    }

    oi_setWheels(0,0);
}

void cal_turn_clockwise(oi_t *sensor, float input_angle)
{

    double mm_traveled = 0;
    double current_angle = 0;

    while(abs(current_angle) < (input_angle - 0.5))
    {
        oi_update(sensor);

        mm_traveled += sensor->distance;
        current_angle += sensor->angle;

        if(mm_traveled > mm_accuracy)
        {
            c_LWP -= 1;
            mm_traveled = 0;
        }

        if(mm_traveled < -(mm_accuracy))
        {
            c_LWP += 1;
            mm_traveled = 0;
        }

        oi_setWheels(c_RWP ,c_LWP); //sets respective wheel speeds
        //lcd_printf("LWP= %d RWP= %d     %f", c_LWP, c_RWP, current_angle);
    }

    oi_setWheels(0,0);
}

void cal_turn(oi_t *sensor, float input_angle)
{
    if(input_angle > 90)
    {
        cal_turn_counter_clockwise(sensor, (input_angle - 90));
    }

    else
    {
        cal_turn_clockwise(sensor, (90 - input_angle));
    }
}

void cal_move_forward(oi_t *sensor, int centimeters)
{

    double mm_traveled = 0;
    double current_angle = 0;

    while(mm_traveled < (centimeters * 10))
    {
        oi_update(sensor);

        mm_traveled += sensor->distance;
        current_angle += sensor->angle;

        //lcd_printf("LWP= %d RWP= %d     %f", forward_LWP, forward_RWP, current_angle);

        if(current_angle > degree_accuracy)
        {
            forward_RWP -= 1;
            current_angle = 0;
        }

        if(current_angle < -(degree_accuracy))
        {
            forward_RWP += 1;
            current_angle = 0;
        }

        oi_setWheels(forward_RWP, forward_LWP);
    }

    oi_setWheels(0,0);
}

void calibrate_wheels(oi_t *sensor)
{
    cal_move_forward(sensor, 200);
    cal_turn_clockwise(sensor, 360);
    cal_turn_counter_clockwise(sensor, 360);

    lcd_printf("f_LWP= %d f_RWP= %d  c_LWP= %d c_RWP= %d cc_LWP= %d cc_RWP= %d",
               forward_LWP, forward_RWP, c_LWP, c_RWP, cc_LWP, cc_RWP);
}
