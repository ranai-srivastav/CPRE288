#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "lcd.h"
#include "open_interface.h"
#include "Timer.h"
#include <stdio.h>
#include "uart_extra_help.h"
#include "calibration.h"

extern volatile double dist_travelled; //distance since last scan

void TapeDrive(oi_t *sensor); // Function incomplete, should be similar to perp2edge in main, but with different scanning attributes.
void Perp2Hole(oi_t *sensor);
void move_forward(oi_t *sensor, int centimeters);
void move_backward(oi_t *sensor, int centimeters);
void turn_clockwise(oi_t *sensor, float degrees);
void turn_counterclockwise(oi_t *sensor, float degrees);
double move_until_bump(oi_t *sensor, int max_cm);
bool hasBumpedLeft(oi_t *sensor);
bool hasBumpedRight(oi_t *sensor);

#endif /* MOVEMENT_H_ */
