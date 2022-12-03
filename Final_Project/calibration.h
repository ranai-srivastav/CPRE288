/*
 * new_movement.h
 *
 *  Created on: Dec 1, 2022
 *      Author: radodge
 */

#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include <inc/tm4c123gh6pm.h>
#include <math.h>
#include "open_interface.h"


extern volatile short cc_RWP;
extern volatile short cc_LWP;

extern volatile short c_RWP;
extern volatile short c_LWP;

extern volatile short forward_RWP;
extern volatile short forward_LWP;


void cal_turn_counter_clockwise(oi_t *sensor, float input_angle);
void cal_turn_clockwise(oi_t *sensor, float input_angle);
void cal_turn(oi_t *sensor, float input_angle);
void cal_move_forward(oi_t *sensor, int centimeters);
void calibrate_wheels(oi_t *sensor);


#endif /* CALIBRATION_H_ */
