#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "setupWifi.h"
#include "lwip/altcp.h"

#define BUF_SIZE 2048
char myBuff[BUF_SIZE];
char header[] = "GET /index.html HTTP/1.1\r\nHOST:example.com\r\n\r\n";
struct connectionState
{
    int state;
    struct altcp_pcb *pcb;
    char *myBuff;
    int start;
};

err_t sent(void *arg, struct altcp_pcb *pcb, u16_t len)
{
    printf("data sent %d\n", len);
}

err_t recv(void *arg, struct altcp_pcb *pcb, 
                                       struct pbuf *p, err_t err)
{
    struct connectionState *cs = (struct connectionState *)arg;
    if (p != NULL)
    {
        printf("recv total %d  this buffer %d next %d err %d\n",
                                 p->tot_len, p->len, p->next, err);
        if ((p->tot_len) > 2)
        {
            pbuf_copy_partial(p, (cs->myBuff) + 
                                      (cs->start), p->tot_len, 0);
            cs->start += p->tot_len;
            cs->myBuff[cs->start] = 0;
            cs->state = 4;
            altcp_recved(pcb, p->tot_len);
        }
        pbuf_free(p);
    }
 
    else
    {
        printf("Connection Closed\n");
        altcp_close(pcb);
        cs->state = 6;
    }
    return ERR_OK;
}

static err_t connected(void *arg, struct altcp_pcb *pcb, err_t err)
{
    struct connectionState *cs = (struct connectionState *)arg;
    cs->state = 2;
    return ERR_OK;
}

err_t poll(void *arg, struct altcp_pcb *pcb)
{
    printf("Connection Closed \n");
    struct connectionState *cs = (struct connectionState *)arg;
    cs->state = 6;
    altcp_close(pcb);
}

void err(void *arg, err_t err)
{
    if (err != ERR_ABRT)
    {
        printf("client_err %d\n", err);
    }
}

err_t newConnection(struct connectionState *cs, char *buf)
{
    cs->state = 0;
    cs->pcb = altcp_new(NULL);
    altcp_recv(cs->pcb, recv);
    altcp_sent(cs->pcb, sent);
    altcp_err(cs->pcb, err);
    altcp_poll(cs->pcb, poll, 10);
    altcp_arg(cs->pcb, cs);
    cs->myBuff = buf;
    cs->start = 0;
    return ERR_OK;
}


int pollConnection(struct connectionState *cs)
{
    switch (cs->state)
    {
    case 0:
    case 1:
    case 3:
    case 6:
        break;
    case 2:
        cs->state = 3;
        cyw43_arch_lwip_begin();
        err_t err = altcp_write(cs->pcb, header,
                                       strlen(header), 0);
        err = altcp_output(cs->pcb);
        cyw43_arch_lwip_end();
        break;
    case 4:
        cs->state = 5;
        break;
    }
    return cs->state;
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

    struct connectionState cs;
    newConnection(&cs, myBuff);

    ip_addr_t ip;
    IP4_ADDR(&ip, 104, 100, 168, 80);
    cyw43_arch_lwip_begin();
    err_t err = altcp_connect(cs.pcb, &ip, 80, connected);
    cyw43_arch_lwip_end();
    cs.state = 1;
    while (true)
    {
        if (pollConnection(&cs) == 6)
        {
            printf("Buffer= %s\n", cs.myBuff);
            cs.state = 0;
            break;
        }
        sleep_ms(1000);
        printf("LED Flash\n");
    }
    printf("Data Transferred\n");
}