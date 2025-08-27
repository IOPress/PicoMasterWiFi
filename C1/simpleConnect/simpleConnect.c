#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"


int setup(uint32_t country, const char *ssid, const char *pass,
                                                  uint32_t auth)
{

   if (cyw43_arch_init_with_country(country))
   {
     return 1;
   }
   cyw43_arch_enable_sta_mode();
   if (cyw43_arch_wifi_connect_blocking(ssid, pass, auth))
   {
     return 2;
   }
}

char ssid[] = "dlink4";
char pass[] = "mypassword";
uint32_t country = CYW43_COUNTRY_mycountry;
uint32_t auth = CYW43_AUTH_WPA2_MIXED_PSK;

int main()
{
    stdio_init_all();
    setup(country, ssid, pass, auth);
    while (true)
    {
        sleep_ms(1);
    }
}