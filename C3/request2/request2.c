#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwip/altcp.h"
#include "setupWifi.h"
#include "request.h"
#define BUF_SIZE 2048
char myBuff1[BUF_SIZE];
char myBuff2[BUF_SIZE];

int main()
{
    stdio_init_all();
    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
        return 1;
    }
    connectWiFi();
    ip_addr_t ip;
    IP4_ADDR(&ip, 104, 100, 168, 80);
    struct connectionState *cs1 = doRequest(&ip, "example.com", 80,
                                            "GET", "/", NULL, myBuff1);
    IP4_ADDR(&ip, 192, 168, 253, 75);
    struct connectionState *cs2 = doRequest(&ip, "192.168.253.75",
                                            80, "PUT", "/", "Hello PUT world", myBuff2);

    while (pollRequest(&cs1) + pollRequest(&cs2))
    {
        sleep_ms(100);
    }
    printf("Both complete\n");
    printf("Buffer 1 = \n%s\n\n", myBuff1);
    printf("Buffer 2 = \n%s\n\n", myBuff2);

    printf("Data Transferred\n");
    cyw43_arch_deinit();
    return 0;
}