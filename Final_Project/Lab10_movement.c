//#include "open_interface.h"
//#include "Timer.h"
//#include <stdio.h>
//#include <math.h>
//#include "movement.h"
//#include "uart_extra_help.h"
//#include "adc.h"
//#include "servo.h"
//#include "ping.h"
//
////volatile  char uart_data;     // Your UART interupt code can place read data here
////volatile  char uart_flag;     // Your UART interupt can update this uart_flag
//
//typedef struct autostruct
//{
//    int minAngle;
//    float objDist;
//}autostruct_t;
//
//
//void sendString(char string[], int length)
//{
//    int i = 0;
//    for(i = 0; i<length; i++)
//    {
//        uart_sendChar(string[i]);
//    }
//}
//
//
//float toRadians(int degrees)
//{
//    return M_PI / 180.0 * degrees;
//}
//
//float est_distance(int IR_sensor_val)
//{
//    // y = 2E+07x^(-1.791) Bot 00
////    return 2*pow(10, 7)*pow(IR_sensor_val, -1.791);
//
//    return 6*pow(10, 6)*pow(IR_sensor_val, -1.795); // Bot 02 y = 6E+06x-1.795
//}
//
//autostruct_t* object_scan(oi_t *sensor)
//{
//    const int line_len = 48;
//    int avg_n = 2;
//
//    int i = 0, j = 0;
//    float sum = 0.0;
//
//    char line[line_len];
//
//    for(i = 0; i<line_len; i++)
//    {
//        line[i] = 0;
//    }
//
//    sprintf(line, "%-10s %-15s\n\r", "Degrees", "IR");
//    sendString(line, line_len);
//
////    float ping_dist[91];
//    float IR_dist[181];
//
//    //scanning from 0-180
//    for(i = 0; i < 181; i++)
//    {
//        sum = 0;
//        for(j = 0; j<avg_n; j++)
//        {
//            servo_move(i);
//            sum = sum + adc_read();     // never actually using the value of sensor_val->IR_raw_val
//        }
//        sum = sum/avg_n;
//
//        sprintf(line, "%-10d %-10.3f\n\r", i, est_distance(sum));
//
//        IR_dist[i] = est_distance(sum);
//        sendString(line, line_len);
//   }
//   servo_move(0);
//   timer_waitMillis(500);
//
//
//    //detecting objects now
//
//    sprintf(line, "%-8s%-8s%-10s%-6s\n\r", "#OBJECT", "Angle", "Ping", "Width");
//    sendString(line, line_len);
//
//    const int edge_detection_threshold = 20;
//    int d = IR_dist[0];
//    int minWidth = 500;
//    int min_angle = 500;
//    int obj = 1;
//    float minDist;
//    avg_n = 5;
//
//    for(i = 1; i<181; i++)
//       {
//           if( abs(d - IR_dist[i]) > edge_detection_threshold)
//           {
//               int count = 0;
//
//               while( i < 181 && (d - IR_dist[i]) > edge_detection_threshold )
//               {
//                   count++;
//                   i++;
//               }
//
//               if(count > 2)
//               {
//                   int oppo_angle = (180-(count))/2; // This is the trig to manage the isoceles triangle
//                   float width = sin(toRadians(count)) * d / (10.0 * sin(toRadians(oppo_angle)));
//
//                   servo_move(i-count/2);
//                   timer_waitMillis(200);
//
//                   sum = 0;
//                   for(j = 0; j<avg_n; j++)
//                   {
//                       sum += ping_dist_est();
//                   }
//                   sum /= avg_n;
//
//                   if(sum < 60)
//                   {
//                       sprintf(line, "%-8d%-8d%-10f%-6.2f\n\r", obj, (i-count-1), sum, width);
//                       sendString(line, line_len);
//
//                       if(minWidth > width)
//                       {
//                           minWidth = width;
//                           min_angle = (i-count/2);
//                           minDist = sum;
//                       }
//                       obj++;
//                   }
//               }
//               else
//               {
//                   i = i - count;
//               }
//               d = IR_dist[i];
//           }
//       }
//
//       servo_move(min_angle);
//       timer_waitMillis(200);
//       autostruct_t *retVal = (autostruct_t*)malloc(sizeof(autostruct_t));
//       retVal->minAngle = min_angle;
//       retVal->objDist = minDist;
//       return retVal;
//
//}
//
//void auto_drive(autostruct_t* minVals, oi_t *sensor)
//{
//    int centerAngle = 90;
//    const int angle_offset = 10;
//
//    if (minVals->minAngle < centerAngle)
//    {
//        turn_clockwise(sensor, (centerAngle - minVals->minAngle) - angle_offset);
//    }
//    else if (minVals->minAngle > centerAngle)
//    {
//        turn_counterclockwise(sensor, (minVals->minAngle - centerAngle) - angle_offset);
//    }
//    if(minVals->objDist > 5)
//    {
//        move_forward(sensor, (minVals->objDist) - 5);
//    }
//
//    free(minVals);
//}
//
//double bump_drive(autostruct_t *minVals, oi_t *sensor)
//{
//    int centerAngle = 90;
//    int angle = centerAngle - minVals->minAngle;
//    //const int angle_offset = 15;
//    double total = 0;
//    char isRight = 0;
//
//    if (minVals->minAngle < centerAngle)
//    {
//        turn_counterclockwise(sensor, (abs(angle)) + angle * 0.5);
//        isRight = 1;
//    }
//    else if (minVals->minAngle > centerAngle)
//    {
//        turn_clockwise(sensor, (angle) - angle * 0.4);
//    }
//    if(minVals->objDist > 5)
//    {
//        total = move_until_bump(sensor, (minVals->objDist)-5);
//
//        if(total < (minVals->objDist)-6)
//        {
//            if( isRight && uart_flag != 1 )
//            {
//                total += move_backward(sensor, 5);
//                turn_clockwise(sensor, 60);
//                move_forward(sensor, 40);
//                turn_counterclockwise(sensor, 90);
//                move_forward(sensor,  20);
//            }
//            else if(uart_flag != 1)
//            {
//                total += move_backward(sensor, 5);
//                turn_counterclockwise(sensor, 60);
//                move_forward(sensor, 40);
//                turn_clockwise(sensor, 90);
//                move_forward(sensor,  20);
//            }
//            else
//            {
//                return 500;
//            }
//        }
////        minVals->objDist = minVals->objDist - total;
//
//
//    }
//    free(minVals);
//    return total;
//}
//
//
//int main()
//{
//    lcd_init();
//    uart_init(115200);
//    oi_t *sensor = oi_alloc();
//    oi_init(sensor);
//    ping_init();
//    servo_init();
//    uart_interrupt_init();
//    adc_init();
//
//    int s = 0, i = 0;
//    char array[15];
//    autostruct_t* min_vals = NULL;
//
//
//    while(s != 'q')
//    {
//        oi_update(sensor);
//        if(uart_flag == 1)
//        {
//            uart_flag = 0;
//            s = uart_data;
//            uart_data = 0;
//        }
//        sprintf(array, "Got an %c", s);
//
//        for(i = 0; i < 15; i++)
//        {
//            uart_sendChar(array[i]);
//        }
//        uart_sendChar('\r');
//
//        if(s == 'w') {
//            oi_setWheels(150, 150);
//        }
//        else if(s == 'a') {
//            oi_setWheels(150, -150);
//        }
//        else if(s == 'd') {
//            oi_setWheels(-150, 150);
//        }
//        else if(s == 's') {
//            oi_setWheels(-150,-150);
//        }
//        else if(s == 'm')
//        {
////            lab3part1(sensor);
//            min_vals = object_scan(sensor);
//            s = 0;
//        }
//        else if(s == 'n')
//        {
//            min_vals = object_scan(sensor);
//            auto_drive(min_vals, sensor);
//            s = 0;
//        }
//        else if(s == 't')
//        {
//            s = 0;
//            min_vals = object_scan(sensor);
//            while ((min_vals->objDist) > 6 && bump_drive(min_vals, sensor) < ((min_vals->objDist)-6))
//            {
//                min_vals = object_scan(sensor);
//            }
//        }
//        else
//        {
//            oi_setWheels(0,0);
//            if(hasBumpedLeft(sensor))
//                uart_sendChar('L');
//
//            if(hasBumpedRight(sensor))
//                uart_sendChar('R');
//        }
//
//    }
//
//    oi_free(sensor);
//}
//
