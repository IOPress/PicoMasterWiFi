#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "setupWifi.h"
#include "pico/aon_timer.h"
#include <time.h>
#include "lwip/apps/sntp.h"
bool state = false;

void SNTPSetClock(u32_t t, u32_t us)
{
    printf("updating clock\n");
    time_t seconds_since_1970 = t - 2208988800;
    struct tm *datetime = gmtime(&seconds_since_1970);
    aon_timer_start_calendar(datetime);
    state = true;
}

int main()
{
    stdio_init_all();
    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
        return 1;
    }
    connect();

    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();

    while (true)
    {
        struct tm t;
        if (state)
        {
            struct tm t;
            aon_timer_get_time_calendar(&t);
            char Date[100];
            strftime(Date, sizeof(Date),
                     "Date: %a, %d %b %Y %k:%M:%S %Z\r\n", &t);
            printf("%s\n", Date);
        }
        sleep_ms(5000);
    }
}