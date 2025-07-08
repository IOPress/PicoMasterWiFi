#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

static int scan_result(void *env, 
                          const cyw43_ev_scan_result_t *result)
{
    if (result)
    {
        printf("ssid: %-32s rssi: %4d chan: %3d mac: %02x:%02x:%02x:%02x:%02x:%02x sec: %u\n",
             result->ssid, result->rssi, result->channel,
              result->bssid[0], result->bssid[1], result->bssid[2],
               result->bssid[3], result->bssid[4], 
                 result->bssid[5],
                  result->auth_mode);
    }
    return 0;
}

int main()
{
    stdio_init_all();

    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    cyw43_wifi_set_up(&cyw43_state, CYW43_ITF_STA,
                                      true, CYW43_COUNTRY_UK);
    cyw43_wifi_scan_options_t scan_options = {0};
    int err = cyw43_wifi_scan(&cyw43_state, &scan_options,
                                           NULL, scan_result);

    while (true)
    {
        if (!cyw43_wifi_scan_active(&cyw43_state))
            break;
        sleep_ms(1000);
        printf("Scan in progress \n");
    }
    printf("Scan Complete\n");
    cyw43_arch_deinit();
    return 0;
}