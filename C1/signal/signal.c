#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "setupWifi.h"

int main()
{
    stdio_init_all();
    connect();
    int32_t rssi;
    while (true)
    {
        sleep_ms(1000);
        cyw43_wifi_get_rssi(&cyw43_state, &rssi);
        printf("rssi: %d \n", rssi);
    }
}