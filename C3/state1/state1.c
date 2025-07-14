#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "setupWifi.h"
#include "lwip/altcp.h"

#define BUF_SIZE 2048
char myBuff[BUF_SIZE];
char header[] = "GET /index.html HTTP/1.1\r\nHOST:example.com\r\n\r\n";

err_t sent(void *arg, struct altcp_pcb *pcb, u16_t len)
{
    printf("data sent %d\n", len);
}

err_t recv(void *arg, struct altcp_pcb *pcb, struct pbuf *p, err_t err)
{

    if (p != NULL)
    {
        printf("recv total %d  this buffer %d next %d err %d\n", p->tot_len, p->len, p->next, err);
        if ((p->tot_len) > 2)
        {
            pbuf_copy_partial(p, myBuff, p->tot_len, 0);
            myBuff[p->tot_len] = 0;
            *(int *)arg = 4;
            altcp_recved(pcb, p->tot_len);
        }
        pbuf_free(p);
    }
    else
    {
        printf("Connection Closed \n");
        altcp_close(pcb);
        *(int *)arg = 6;
    }
    return ERR_OK;
}

static err_t altcp_client_connected(void *arg, struct altcp_pcb *pcb, err_t err)
{
    *(int *)arg = 2;
    return ERR_OK;
}

err_t poll(void *arg, struct altcp_pcb *pcb)
{
    printf("Connection Closed \n");
    *(int *)arg = 6;
    altcp_close(pcb);
}

void err(void *arg, err_t err)
{
    if (err != ERR_ABRT)
    {
        printf("client_err %d\n", err);
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

    int state = 0;
    struct altcp_pcb *pcb = altcp_new(NULL);

    altcp_recv(pcb, recv);
    altcp_sent(pcb, sent);
    altcp_err(pcb, err);
    altcp_poll(pcb, poll, 10);
    altcp_arg(pcb, &state);

    ip_addr_t ip;
    IP4_ADDR(&ip, 104, 100, 168, 80);
    cyw43_arch_lwip_begin();
    err_t err = altcp_connect(pcb, &ip, 80, altcp_client_connected);
    cyw43_arch_lwip_end();
    state = 1;

    while (state != 0)
    {
        switch (state)
        {
        case 2:
            state = 3;
            cyw43_arch_lwip_begin();
            err = altcp_write(pcb, header, strlen(header), 0);
            err = altcp_output(pcb);
            cyw43_arch_lwip_end();
            break;
        case 4:
            state = 5;
            break;
        case 6:
            printf("Buffer= %s\n", myBuff);
            state = 0;
            break;
        default:
            sleep_ms(1000);
            printf("LED Flash\n");
        }
    }
    printf("Data Transfered\n");
}