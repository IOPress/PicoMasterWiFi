#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwip/altcp.h"
#include "setupWifi.h"
#include "request.h"
#define BUF_SIZE 2048
char myBuff[BUF_SIZE];

int main()
{
    stdio_init_all();
    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
        return 1;
    }
    connect();
    char randdata[500];
    ip_addr_t ip;
    IP4_ADDR(&ip, 192, 168, 253, 75);

    for (int p = 0; p < 500; p++)
    {
        randdata[p] = (uint8_t)rand();
    }

    IP4_ADDR(&ip, 192, 168, 253, 75);
    struct connectionState *cs = doRequestBinary(&ip, "192.168.253.75", 80, "PUT", "/", randdata, 500, myBuff);
    while (pollRequest(&cs))
    {
        sleep_ms(100);
    }
    printf("Buffer= %s\n", myBuff);
    printf("Data Transferred\n");
}