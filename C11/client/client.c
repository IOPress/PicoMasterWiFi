#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "setupWifi.h"
#include <FreeRTOS.h>
#include <task.h>
#include "pico/lwip_freertos.h"
#include "lwip/tcp.h"

#define BUF_SIZE 2048
char myBuff[BUF_SIZE];
char header[] = "GET /index.html HTTP/1.1\r\nHOST:example.com\r\n\r\n";

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

void mainTask(void *arg)
{

    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
    }
    connectWiFi();

    struct tcp_pcb *pcb = tcp_new();
    tcp_recv(pcb, recv);
    ip_addr_t ip;
    IP4_ADDR(&ip, 104, 100, 168, 80);
    cyw43_arch_lwip_begin();
    err_t err = tcp_connect(pcb, &ip, 80, connected);
    cyw43_arch_lwip_end();

    while (true)
    {
        sleep_ms(500);
    }
}

int main()
{
    stdio_init_all();
    sleep_ms(10);
    TaskHandle_t maintask;
    UBaseType_t uxCoreAffinityMask;
    xTaskCreate(mainTask, "mainTask", 2048, NULL, 1, &maintask);
    uxCoreAffinityMask = 1 << 1;
    vTaskCoreAffinitySet(maintask, uxCoreAffinityMask);
    vTaskStartScheduler();
    while (true)
    {
        sleep_ms(500);
    }
}