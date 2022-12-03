#ifndef SERVO_H
#define SERVO_H

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"


void servo_init(void);

void servo_move(int degrees);



#endif /*SERVO_H*/
