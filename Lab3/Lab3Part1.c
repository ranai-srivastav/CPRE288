//#include "lcd.c"
#include "cyBot_uart.h"
#include "open_interface.h"
#include "Timer.h"
#include "cyBot_Scan.h"
#include <stdio.h>
#include <math.h>

void sendString(char string[], int length)
{
    int i = 0;
    for(i = 0; i<length; i++)
    {
        cyBot_sendByte(string[i]);
    }
}


void lab3part1(oi_t *sensor)
{
    int i = 0, j = 0;
    cyBOT_Scan_t *sensor_val = malloc(sizeof(cyBOT_Scan_t));
    char line[38];
    sprintf(line, "%-10s%-18s\n\r", "Degrees", "Distance (cm)");
    sendString(line, 30);
    float dist[91];

    for(i = 0; i < 91; i++)
    {
        cyBOT_Scan(i*2, sensor_val);

        sprintf(line, "%-10d%-18f\n\r", i*2, sensor_val->sound_dist);
        dist[i] = sensor_val->sound_dist;
        sendString(line, 30);
   }

    sprintf(line, "%-8s%-8s%-10s%-6s\n\r", "#OBJECT", "Angle", "Distance", "Width");
    sendString(line, 35);


    const int edge_detection_threshold = 25;
    int d = dist[0];
    int minWidth = 500;
    int minAngle = -1;
    int obj = 1;

    //detecting objects now
    for(i = 1; i<90; i++)
    {
        if( abs(d - dist[i]) > edge_detection_threshold)
        {
            int count = 0;

            while( i < 91 && (d - dist[i]) > edge_detection_threshold )
            {
                count++;
                i++;
            }

            if(count > 3)
            {

                sprintf(line, "%-8d%-8d%-10d%-6d\n\r", obj, (i-count-1)*2, d, count*2);
                sendString(line, 35);

                if(minWidth > count*2)
                {
                    minWidth = count*2;
                    minAngle = (i-1)*count;
                }
                obj++;
            }
            else
            {
                i = i - count;
            }
            d = dist[i];
        }
    }


    cyBOT_Scan(minAngle, NULL);


}

float toRadians(int degrees)
{
    return M_PI / 180.0 * degrees;
}

float est_distance(int IR_sensor_val)
{
    return 2*pow(10, 7)*pow(IR_sensor_val, -1.987);
}



void lab3part2(oi_t *sensor)
{
    const int line_len = 48;
    int i = 0;
    cyBOT_Scan_t *sensor_val = malloc(sizeof(cyBOT_Scan_t));
    char line[line_len];
    sprintf(line, "%-10s%-15s%-15s\n\r", "Degrees", "Ping", "IR");
    sendString(line, line_len);
    float ping_dist[91];
    float IR_dist[91];

    for(i = 0; i < 91; i++)
    {
        cyBOT_Scan(i*2, sensor_val);

        sprintf(line, "%-10d%-10.3f%-10.3f\n\r", i*2, sensor_val->sound_dist, est_distance(sensor_val->IR_raw_val));
        ping_dist[i] = sensor_val->sound_dist;
        IR_dist[i] = est_distance(sensor_val->IR_raw_val);
        sendString(line, line_len);
   }

    sprintf(line, "%-8s%-8s%-10s%-10s%-6s\n\r", "#OBJECT", "Angle", "Ping", "IR", "Width");
    sendString(line, line_len);


    const int edge_detection_threshold = 25;
    int d = IR_dist[0];
    int minWidth = 500;
    int obj = 1;

    //detecting objects now
    for(i = 1; i<90; i++)
    {
        if( abs(d - IR_dist[i]) > edge_detection_threshold)
        {
            int count = 0;

            while( i < 91 && (d - IR_dist[i]) > edge_detection_threshold )
            {
                count++;
                i++;
            }

            if(count > 3)
            {
                int y = (180-(count*2))/2;
                float width = sin(toRadians(count*2)) * d / (10.0 * sin(toRadians(y)));

                sprintf(line, "%-8d%-8d%-10f%-6.2f\n\r", obj, (i-count-1)*2, IR_dist[i-count/2], width);
                sendString(line, line_len);

                if(minWidth > width)
                {
                    minWidth = width;
                }
                obj++;
            }
            else
            {
                i = i - count;
            }
            d = IR_dist[i];
        }
    }

    cyBOT_Scan(0, NULL);
}

int main()
{
    lcd_init();
    cyBot_uart_init();
    int s = 0;
    char array[15];
    int i = 0;

    oi_t *sensor = oi_alloc();
    oi_init(sensor);
    cyBOT_init_Scan(0b0111);

    while(s != 'q') {
        oi_update(sensor);
        s = cyBot_getByte();
        lcd_clear();
        sprintf(array, "Got an %c", s);

        for(i = 0; i < 15; i++) {
            cyBot_sendByte(array[i]);
        }
        cyBot_sendByte('\r');

        lcd_putc(s);


        if(s == 'w') {
            oi_setWheels(150, 150);
        }
        else if(s == 'a') {
            oi_setWheels(150, -150);
        }
        else if(s == 'd') {
            oi_setWheels(-150, 150);
        }
        else if(s == 's') {
            oi_setWheels(-150,-150);
        }
        else if(s == 'm')
        {
//            lab3part1(sensor);
            lab3part2(sensor);
        }
        else
        {
//            oi_free(sensor);
            oi_setWheels(0,0);
        }



    }

    oi_free(sensor);
}






