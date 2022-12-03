/*
 * movement.h
 *
 *  Created on: Dec 2, 2022
 *      Author: radodge
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "lcd.h"
#include "open_interface.h"
#include "Timer.h"
#include <stdio.h>
#include "uart_extra_help.h"
#include "Our_Functions.h"
#include "calibration.h"

void move_forward(oi_t *sensor, int centimeters);
void move_backward(oi_t *sensor, int centimeters);
void turn_clockwise(oi_t *sensor, float degrees);
void turn_counterclockwise(oi_t *sensor, float degrees);
double move_until_bump(oi_t *sensor, int max_cm);
int Perp2Tape(oi_t *sensor);
void Perp2Hole(oi_t *sensor);
void TapeDrive(oi_t *sensor);

#endif /* MOVEMENT_H_ */
