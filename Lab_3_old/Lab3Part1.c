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



void lab3part2(oi_t *sensor)
{
    int i = 0;
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
                int y = (180-(count*2))/2;
                float width = sin(toRadians(count*2)) * d / (10.0 * sin(toRadians(y)));

                sprintf(line, "%-8d%-8d%-10d%-6.2f\n\r", obj, (i-count-1)*2, d, width);
                sendString(line, 36);

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
            d = dist[i];
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
    cyBOT_init_Scan(0b0011);

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
            oi_setWheels(250, 250);
        }
        else if(s == 'a') {
            oi_setWheels(250, -250);
        }
        else if(s == 'd') {
            oi_setWheels(-250, 250);
        }
        else if(s == 's') {
            oi_setWheels(-250,-250);
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






