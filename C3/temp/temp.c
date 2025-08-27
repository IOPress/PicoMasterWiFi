#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwip/altcp.h"
#include "setupWifi.h"
#include "request.h"
#define BUF_SIZE 4096
char myBuff[BUF_SIZE];

int readTemp()
{
    return 33;
}

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
    IP4_ADDR(&ip, 192, 168, 253, 75);
    while (true)
    {
        int t = readTemp();
        int len = snprintf(NULL, 0, "%d", t);
        char *requestData = malloc(len + 1);
        snprintf(requestData, len + 1, "%d", t);
        printf("%s\n", requestData);
        struct connectionState *cs1 = doRequest(&ip,
                                                "192.168.253.75", 80,
                                                "PUT", "/", requestData, myBuff);
        while (pollRequest(&cs1))
        {
            sleep_ms(200);
        }
        printf("%s\n", myBuff);
        sleep_ms(5000);
    }
    return 0;
}