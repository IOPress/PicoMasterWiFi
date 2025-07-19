#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwip/altcp.h"
#include "lwip/altcp_tls.h"

#include "setupWifi.h"

#define BUF_SIZE 2048
char myBuff[BUF_SIZE];
char header[] = "GET /index.html HTTP/1.1\r\nHOST:example.com\r\n\r\n ";

err_t recv(void *arg, struct altcp_pcb *pcb,
           struct pbuf *p, err_t err)
{
    if (p != NULL)
    {
        printf("recv total %d  this buffer %d next %d err %d\n",
               p->tot_len, p->len, p->next, err);
        pbuf_copy_partial(p, myBuff, p->tot_len, 0);
        myBuff[p->tot_len] = 0;
        printf("Buffer= %s\n", myBuff);
        altcp_recved(pcb, p->tot_len);
        pbuf_free(p);
    }
    return ERR_OK;
}

static err_t altcp_client_connected(void *arg,
                                    struct altcp_pcb *pcb, err_t err)
{
    err = altcp_write(pcb, header, strlen(header), 0);
    err = altcp_output(pcb);
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
    connect();

    struct altcp_tls_config *tls_config = altcp_tls_create_config_client(NULL, 0);
    struct altcp_pcb *pcb = altcp_tls_new(tls_config, IPADDR_TYPE_ANY);
    mbedtls_ssl_set_hostname(altcp_tls_context(pcb), "example.com");
    altcp_recv(pcb, recv);

    ip_addr_t ip;
    IP4_ADDR(&ip, 104, 100, 168, 80);
    cyw43_arch_lwip_begin();
    err_t err = altcp_connect(pcb, &ip, 443, altcp_client_connected);
    cyw43_arch_lwip_end();
    while (true)
    {
        sleep_ms(500);
    }
}