#ifndef PING_H
#define PING_H

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"

extern volatile int overflow_count;

void ping_init(void);

void ping_interrupt_handler(void);

float ping_dist_est();

void ping_read();

void Timer3_handler();


#endif //PING_H
