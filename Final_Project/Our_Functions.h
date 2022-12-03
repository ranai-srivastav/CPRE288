/*
 * Our_Functions.h
 *
 *  Created on: Oct 3, 2022
 *      Author: nayers99
 */

#ifndef OUR_FUNCTIONS_H_
#define OUR_FUNCTIONS_H_

#include "Our_Functions.h"
#include <math.h>
#include "lcd.h"
#include "open_interface.h"
//#include "movement.h"


void int2char(int input);

int Raw2dist(int IR_val);

//int averagedIRdist(void);

int Perp2Tape(oi_t *sensor);

#endif /* OUR_FUNCTIONS_H_ */
