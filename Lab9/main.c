#include "ping.h"
#include "Timer.h"
#include "lcd.h"

int main(void)

{
    ping_init();
    lcd_init();

    int clockCounts = 0;
    float estDist = 0;
    char line[40];
    while(1)
    {
        clockCounts = ping_read();
        estDist = dist_est(clockCounts);
        sprintf(line, "%-6d              %-5.2f", clockCounts, estDist);
        lcd_clear();
        lcd_puts(line);
        timer_waitMillis(1000);

    }
	return 0;
}
