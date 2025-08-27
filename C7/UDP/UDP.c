#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "setupWifi.h"
int main()
{
    stdio_init_all();
     if (cyw43_arch_init())
 {
 printf("failed to initialise\n");
 return 1;
 }
    connectWiFi();
    struct udp_pcb *pcb = udp_new();
    udp_bind(pcb, IP_ADDR_ANY, 8080);
    ip_addr_t ip;
    IP4_ADDR(&ip, 192, 168, 253, 75);
    udp_connect(pcb, &ip, 8080);
    char message[] = "Hello UDP World";
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, 
                              strlen(message) + 1, PBUF_RAM);
    snprintf(p->payload, strlen(message) + 1, "%s", message);
    err_t er = udp_send(pcb, p);  
    pbuf_free(p);
    while (true)
    {
        sleep_ms(500);
    }
}