#ifndef SERVO_H
#define SERVO_H

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "math.h"
#include "Our_Functions.h"
#include "adc.h"
#include "ping.h"


typedef struct{
    int structangle;
    int structIRdistance;
    int structPingdistance;
    //Scanned data is assigned to a variable of this struct, there will 91 instaqnces to account for all scans.
}scan_data;

void servo_init(void);

void servo_move(int degrees);

void servo_rotate(int angle);

scan_data *ScanVals(int startAngle, int endAngle);

#endif //SERVO_H
