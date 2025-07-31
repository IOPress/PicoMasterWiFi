#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/altcp_tcp.h"
#include "pico/aon_timer.h"
#include "time.h"
#include "setupWifi.h"

#define BUF_SIZE 2048


void sendData(struct altcp_pcb *pcb)
{
    err_t err;
    char html[] = "<html><head><title>Temperature</title></head><body><p>{\"humidity\":81%,\"airtemperature\":23.5C}</p></body></html>\r\n";
    char headers[1024] = {0};
    char Status[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html;charset=UTF-8\r\nServer:Picow\r\n";

    struct tm t;
    aon_timer_get_time_calendar(&t);
    char Date[100];
    strftime(Date, sizeof(Date),
             "Date: %a, %d %b %Y %k:%M:%S %Z\r\n", &t);
    char ContLen[100] = {0};
    snprintf(ContLen, sizeof ContLen, "Content-Length:%d \r\n",
             strlen(html));
    snprintf(headers, sizeof headers, "%s%s%s\r\n",
             Status, Date, ContLen);
    char data[2048] = {0};
    snprintf(data, sizeof data, "%s%s", headers, html);
    err = altcp_write(pcb, data, strlen(data), 0);
    err = altcp_output(pcb);
}

err_t recv(void *arg, struct altcp_pcb *pcb,
           struct pbuf *p, err_t err)
{
    char myBuff[BUF_SIZE];
    if (p != NULL)
    {
        printf("recv total %d  this buffer %d next %d err %d\n",
               p->tot_len, p->len, p->next, err);
        pbuf_copy_partial(p, myBuff, p->tot_len, 0);
        myBuff[p->tot_len] = 0;
        printf("Buffer= %s\n", myBuff);
        altcp_recved(pcb, p->tot_len);
        pbuf_free(p);
        sendData(pcb);
    }
    return ERR_OK;
}

static err_t sent(void *arg, struct altcp_pcb *pcb, u16_t len)
{
    altcp_close(pcb);
}

static err_t accept(void *arg, struct altcp_pcb *pcb, err_t err)
{
    altcp_recv(pcb, recv);
    altcp_sent(pcb, sent);
    printf("connect!\n");
    return ERR_OK;
}

void setTimeDate()
{
    struct tm t = {
        .tm_year = 2025,
        .tm_mon = 02,
        .tm_mday = 03,
        .tm_wday = 5,
        .tm_hour = 11,
        .tm_min = 10,
        .tm_sec = 00};
   aon_timer_start_calendar(&t);
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
    setTimeDate();


    struct altcp_pcb *pcb = altcp_new(NULL);
    altcp_accept(pcb, accept);
    altcp_bind(pcb, IP_ADDR_ANY, 80);
    cyw43_arch_lwip_begin();
    pcb = altcp_listen_with_backlog(pcb, 3);
    cyw43_arch_lwip_end();
    while (true)
    {
        sleep_ms(500);
    }
}