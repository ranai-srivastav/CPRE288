#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "servo.h"

unsigned int Val_180 = 37355; // Cybot 7
unsigned int Val_0 = 7881; //Cybot 7
unsigned int servo_period = 320000; // 20 ms
extern volatile int edge;
volatile unsigned int fallingedge;
volatile unsigned int risingedge;
int edgetime;


void servo_init(void)
{

    SYSCTL_RCGCGPIO_R   |= 0b000010;    //Enable GPIO PB Clk
    SYSCTL_RCGCTIMER_R  |= 0b000010;    //Enable Timer 1 Clk

    while((SYSCTL_PRGPIO_R  & 0b0010) == 0){};   //Waiting for the reset to go through
    while((SYSCTL_PRTIMER_R & 0b0010) == 0){};

    GPIO_PORTB_DEN_R    |= 0b00100000;      //Enable Digital Function
    GPIO_PORTB_DIR_R    |= 0b00100000;      //Set PB5 as output
    GPIO_PORTB_AFSEL_R  |= 0b00100000;      //Enable Alternate Function for PB5
    GPIO_PORTB_PCTL_R   &= 0xFF0FFFFF;      //Clear PCTL for PB5
    GPIO_PORTB_PCTL_R   |= 0x00700000;      //Alternate Function set to T1CCP1

    TIMER1_CTL_R        &= 0b011111111;     //Disable Timer1 B (TBEN)
    TIMER1_CFG_R        = 0x00000004;       //16 bit timer config
    TIMER1_TBMR_R       &= 0xFFFFFFF0;      //Clears bits 3:0
    TIMER1_TBMR_R       |= 0b1010;          //PWM Mode, Edge-Count Mode, Periodic Timer Mode
    TIMER1_CTL_R        &= 0xFFFFBFFF;      //Output is not inverted
    //TIMER1_CTL_R        |= 0x4000;          //Output is inverted
    TIMER1_TBPR_R       = 0x04;             //Extension of timer (0b100)
    TIMER1_TBILR_R      = 0b1110001000000000; //0xE200;           //Set first 16 bits of timer period (1110001000000000)
    TIMER1_TBMATCHR_R   = 0x5355;           //283,477 = 0xA380;            //0b1001010001110000000 = 320000 - 16000;   //Initialize servo at 0 degrees
    TIMER1_TBPMR_R      = 0x4;
    TIMER1_CTL_R        |= 0b100000000;     //Re-Enable Timer1 B
}

void servo_rotate(int angle){
    int clock_cycles;
    //clock_cycles = round(((angle * (5.55555 * pow(10, -6))) + .001) * (16.0 * pow(10, 6)));
    clock_cycles = round((((Val_180 - Val_0) / 180.0) * angle) + Val_0);
    //int2char(clock_cycles);
    //int2char(clock_cycles);

    TIMER1_TBMATCHR_R = servo_period - clock_cycles; //total period - pulse width
    TIMER1_TBPMR_R = ((servo_period - clock_cycles) >> 16);

    //int2char((0x055F00 - clock_cycles));

}


scan_data_t *ScanVals(int startAngle, int endAngle)
{
    int i = 0, j, sum = 0;
    int angle;
    int IR_dist;
    static scan_data_t dataarray[91];
    int IRscans = 10;

    for(angle = startAngle; angle <= endAngle; angle += 2)
    {
        servo_rotate(angle);
        //int2char(IR_dist);
        timer_waitMillis(50);
        for(j = 0; j < IRscans; j++){
            sum += adc_read();
        }
        IR_dist = Raw2dist(sum / IRscans);
        sum = 0;
        dataarray[i].structIRdistance = IR_dist;

        ping_read();
        while(edge == 0){

        }
        risingedge = TIMER3_TBR_R;
        while(edge == 1){

        }
        fallingedge = TIMER3_TBR_R;
        edgetime = abs(fallingedge - risingedge);

        dataarray[i].structPingdistance = (50 * 343.0 * edgetime) / (16 * pow(10,6));

        dataarray[i].structangle = angle;

        i += 1;
    }
    i = 0;
    return dataarray;
}



