/*
 * new_movement.h
 *
 *  Created on: Dec 1, 2022
 *      Author: radodge
 */

#ifndef NEW_MOVEMENT_H_
#define NEW_MOVEMENT_H_

#include <inc/tm4c123gh6pm.h>
#include "open_interface.h"
#include "math.h"

/*
extern volatile short cc_RWP = 50;
extern volatile short cc_LWP = -50;

extern volatile short c_RWP = -50;
extern volatile short c_LWP = 50;

extern volatile short forward_RWP = 50;
extern volatile short forward_LWP = 50;
*/

void cal_turn_counter_clockwise(oi_t *sensor, float input_angle);
void cal_turn_clockwise(oi_t *sensor, float input_angle);
void cal_turn(oi_t *sensor, float input_angle);
void cal_move_forward(oi_t *sensor, int centimeters);
void calibrate_wheels(oi_t *sensor);


#endif /* NEW_MOVEMENT_H_ */
