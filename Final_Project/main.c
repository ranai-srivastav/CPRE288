#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"
#include "button.h"

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

int main(void)
{
    lcd_init();
    oi_t *sensor = oi_alloc();
    oi_init(sensor);
    button_init();

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

void perp_to_edge(oi_t *sensor)
{
    oi_setWheels(50, 50);

    while(sensor->cliffLeftSignal < 2700 || sensor->cliffRightSignal < 2700){oi_update(sensor);}

    oi_setWheels(0, 0);
    oi_update(sensor);

    while(sensor->cliffLeftSignal < 2700)
    {
        oi_setWheels(0, 25);
        oi_update(sensor);
    }

    while(sensor->cliffRightSignal < 2700)
    {
        oi_setWheels(25, 0);
        oi_update(sensor);
    }

    if(sensor->cliffLeftSignal < 2700 || sensor->cliffRightSignal < 2700)
    {
        oi_setWheels(-50, -50);
        oi_update(sensor);
        timer_waitMillis(1000);
        3perp_to_edge(sensor);

    }

}

//exit_condition move_until_exit(oi_t *sensor)
//{
//
//}
