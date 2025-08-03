#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "setupWifi.h"

#define BUF_SIZE 1024

void recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    char myBuff[BUF_SIZE];
    if (p != NULL)
    {
        printf("recv total %d  this buffer %d next %d \n", p->tot_len, p->len, p->next);
        printf("From %s:%d\n", ipaddr_ntoa(addr), port);
        pbuf_copy_partial(p, myBuff, p->tot_len, 0);
        myBuff[p->tot_len] = 0;
        printf("Buffer= %s\n", myBuff);
        pbuf_free(p);
    }
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

    struct udp_pcb *pcb = udp_new();
    udp_recv(pcb, recv, NULL);

    udp_bind(pcb, IP_ADDR_ANY, 8080);

    //   ip_addr_t ip;
    //  IP4_ADDR(&ip, 192, 168, 11, 101);
    // IP4_ADDR(&ip, 192, 168, 11, 255);
    // udp_connect(pcb, &ip, 8080);

    while (true)
    {
        sleep_ms(500);
    }
}