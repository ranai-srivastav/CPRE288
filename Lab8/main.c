//#include "adc.h"
//#include "uart_extra_help.h"
//#include "Timer.h"
//#include "lcd.h"
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
//int main(void)
//{
//	adc_init();
//	uart_init(115200);
//	uart_interrupt_init();
//	lcd_init();
//	char s = 0;
//
//	while(s != 'q')
//	{
//	//        s = uart_receive(); //TODO our UART Code
//        if(flag == 1)
//        {
//            flag = 0;
//            s = uart_data;
//            uart_data = 0;
//        }
//	    int sensor_val = adc_read();
//	    char ret_val[20];
//	    sprintf(ret_val, "Raw: %d \r\n", sensor_val);
//
//	    sendString(ret_val, 20);
//	}
//}
