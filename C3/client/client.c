#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "setupWifi.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"

#define BUF_SIZE 2048
char myBuff[BUF_SIZE];
char header[] = "GET /index.html HTTP/1.1\r\nHOST:example.com\r\n\r\n";

void dns_found(const char *name, const ip_addr_t *ip, void *arg)
{
    ip_addr_t *ipResult = (ip_addr_t *)arg;
    if (ip)
    {
        ip4_addr_copy(*ipResult, *ip);
    }
    else
    {
        ip4_addr_set_loopback(ipResult);
    }
    return;
}

err_t getIP(char *URL, ip_addr_t *ipResult)
{
    cyw43_arch_lwip_begin();
    err_t err = dns_gethostbyname(URL, ipResult,
                                  dns_found, ipResult);
    cyw43_arch_lwip_end();
    return err;
}

err_t recv(void *arg, struct tcp_pcb *pcb,
           struct pbuf *p, err_t err)
{
    if (p != NULL)
    {
        printf("\nrecv total %d  this buffer %d next %d err %d\n",
               p->tot_len, p->len, p->next, err);
        pbuf_copy_partial(p, myBuff, p->tot_len, 0);
        myBuff[p->tot_len] = 0;
        printf("Buffer= %s\n", myBuff);
        tcp_recved(pcb, p->tot_len);
        pbuf_free(p);
    }
    return ERR_OK;
}

err_t connected(void *arg, struct tcp_pcb *pcb, err_t err)
{
    err = tcp_write(pcb, header, strlen(header), 0);
    err = tcp_output(pcb);
    return ERR_OK;
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

    struct tcp_pcb *pcb = tcp_new();
    tcp_recv(pcb, recv);
    
    ip_addr_t ip;
    ip4_addr_set_zero(&ip);
    getIP("example.com", &ip);
    while (!ip_addr_get_ip4_u32(&ip))
    {
        sleep_ms(100);
    };
    if (ip4_addr_isloopback(&ip))
    {
        printf("address not found");
    }

    cyw43_arch_lwip_begin();
    err_t err = tcp_connect(pcb, &ip, 80, connected);
    cyw43_arch_lwip_end();

    while (true)
    {
        sleep_ms(500);
    }
}
