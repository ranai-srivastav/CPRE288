/*
 * state.h
 *
 *  Created on: Dec 2, 2022
 *      Author: ranais
 */

#ifndef STATE_H_
#define STATE_H_

#include "movement.h"
#include "servo.h"

extern volatile double dist_travelled;
void perp_to_edge(oi_t *sensor);



#endif /* STATE_H_ */
