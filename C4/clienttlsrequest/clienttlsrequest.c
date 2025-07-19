#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/altcp_tcp.h"
#include "lwip/altcp_tls.h"
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

    ip_addr_t ip;
    IP4_ADDR(&ip, 104, 100, 168, 80);

    struct connectionState *cs1 = doTLSRequestBinary(&ip,
                                                     "example.com", 443, "GET", "/", NULL, 0, myBuff);
    while (pollRequest(&cs1))
    {
        sleep_ms(200);
    }
    printf("%s\n", myBuff);

    return 0;
}