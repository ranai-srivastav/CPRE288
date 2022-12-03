#include "servo.h"
#include "lcd.h"
#include "button.h"

int main(void)
{
    

    calibrate_wheels(sensor);

    oi_free(sensor);

	return 0;
}

//exit_condition move_until_exit(oi_t *sensor)