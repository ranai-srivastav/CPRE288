#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "servo.h"

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

void servo_move(int degree)
{
    // calibrated to cybot02
    int counts = 151.59 * degree + 284366;

    TIMER1_TBMATCHR_R = counts;
    TIMER1_TBPMR_R = counts >> 16;

    //TODO wait until move is complete
}



