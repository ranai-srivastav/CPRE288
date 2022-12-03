#include "servo.h"
#include "lcd.h"
#include "button.h"
#include "state.h"
#include "calibration.h"
#include "ping.h"
#include "adc.h"

volatile double dist_travelled; //distance since last scan
volatile int edge;


int main(void)
{
    
    lcd_init();
    uart_init(115200);
    ping_init();
    servo_init();
    uart_interrupt_init();
    adc_init();

    oi_t *sensor = oi_alloc();
    oi_init(sensor);

    calibrate_wheels(sensor);

    oi_free(sensor);

	return 0;
}

//exit_condition move_until_exit(oi_t *sensor)
