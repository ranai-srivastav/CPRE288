#ifndef PING_H
#define PING_H

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"

void ping_init(void);

void ping_interrupt_init(void);

void ping_interrupt_handler(void);

int ping_read(void);

float dist_est(int totalCounts);

#endif PING_H
