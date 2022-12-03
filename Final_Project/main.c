//#include "servo.h"
//#include "lcd.h"
//#include "button.h"
//
//int main(void)
//{
//    lcd_init();
//    servo_init();
//    button_init();
//    init_button_interrupts();
//
//    int i = 0;
//    int curr_angle = 0;
//    int dir = 1;
//
//
//    while(1)
//    {
//        if(button_event)
//        {
//            lcd_clear();
//            switch(button_getButton())
//            {
//            case 1:
//                curr_angle += dir * 1;
//                break;
//            case 2:
//                curr_angle += dir * 5;
//                break;
//            case 3:
//                dir = -dir;
//                break;
//            case 4:
//                curr_angle = 90 + dir * 90;
//                break;
//            }
//
//            if(curr_angle > 180)
//            {
//                curr_angle = 180;
//            }
//            else if(curr_angle < 0)
//            {
//                curr_angle = 0;
//            }
//
//            char line[40];
//            sprintf(line, "T1B Match %07d    E_Ang: %03d   Dir:%2d", (TIMER1_TBPMR_R<<16) + TIMER1_TBMATCHR_R, curr_angle, dir);
//            lcd_puts(line);
//            servo_move(curr_angle);
//            button_event = 0;
//        }
//    }
//
//
//
////    for (i = 0; i < 181; i++)
////    {
////        //TODO Figure out how to wait for servo to finish moving
////    }
//
//
//	return 0;
//}
