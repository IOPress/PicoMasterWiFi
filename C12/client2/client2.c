
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include <FreeRTOS.h>
#include <task.h>
#include "pico/lwip_freertos.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "setupWifi.h"

#include "lwip/dns.h"

typedef struct
{
    char *url;
    char *buffer;
    int len;
    bool done;
} Page;

void dns_found(const char *name, const ip_addr_t *ip, void *arg)
{
    ip_addr_t *ipResult = (ip_addr_t *)arg;
    if (ip)
    {
        ip4_addr_copy(*ipResult, *ip);
    }
    else
    {
        ip4_addr_set_zero(ipResult);
    }
    return;
}

ip_addr_t DNS(char URL[])
{
    ip_addr_t ipResult;
    err_t err = dns_gethostbyname(URL, &ipResult, dns_found, &ipResult);
    if (err == ERR_OK)
    {
        return ipResult;
    }
    ip4_addr_set_loopback(&ipResult);
    while (ip4_addr_isloopback(&ipResult))
    {
        vTaskDelay(50);
    }
    return ipResult;
}

void asyncgetPage(void *arg)
{
    err_t err;
    Page *page = (Page *)arg;

    ip_addr_t ipResult = DNS(page->url);
    if (ipResult.addr == 0)
        printf("dns fail\n");
    else
        printf("got dns %X\n", ipResult);

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    printf("created socket %d\n", sockfd);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
   // addr.sin_addr.s_addr = inet_addr("23.192.228.80");
    addr.sin_addr.s_addr = ipResult.addr;

    err = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (err < 0)
    {
        printf("no connection %X\n", errno);
        printf("close socket %d\n", sockfd);
        close(sockfd);
        vTaskDelete(NULL);
    }
    else
        printf("conntect \n");

    char header[] = "GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n ";

    int n = write(sockfd, header, strlen(header));
    if (n < 0)
    {
        printf("no write %X\n", errno);
        printf("close socket %d\n", sockfd);
        close(sockfd);
        vTaskDelete(NULL);
    }
    else
        printf("data sent socket %d %d\n", sockfd, n);

    fd_set rfds;
    struct timeval tv;
    do
    {
        FD_ZERO(&rfds);
        FD_SET(sockfd, &rfds);
        tv.tv_sec = 2;
        tv.tv_usec = 0;
        n = select(5, &rfds, NULL, NULL, &tv);
        if (n < 0)
        {
            printf("read error %X\n", errno);
            break;
        }
        if (n == 0)
        {
            printf("read timeout\n");
            break;
        }
        n = read(sockfd, page->buffer, page->len);
        if (n < 0)
        {
            printf("read error %X\n", errno);
            break;
        }
        page->buffer[n] = 0;
        printf("\ndata recieved on socket %d  %d\n\n", sockfd, n);
    } while (true);

    printf("close socket %d\n", sockfd);
    close(sockfd);
    page->done = true;
    vTaskDelete(NULL);
}

void getPage(Page *page)
{
    TaskHandle_t asyncgetpage;
    xTaskCreate(asyncgetPage, "asyncgetPage", 2048, page, 0, &asyncgetpage);
    return;
}

Page initPage(char url[], char buffer[], int len)
{
    Page page;
    page.url = url;
    page.buffer = buffer;
    page.len = len;
    page.done = false;
    return page;
}

void mainTask(void *arg)
{
    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
    }
    connectWiFi();   

    char buffer1[2000];
    char url[] = "example.com";
    Page pagetemp1 = initPage(url, buffer1, 2000);
    getPage(&pagetemp1);

    char buffer2[2000];
    Page pagetemp2 = initPage(url, buffer2, 2000);
    getPage(&pagetemp2);
    while (true)
    {
        if (pagetemp1.done)
        {
            printf("\nfirst request \n %s\n", buffer1);
            pagetemp1.done = false;
        }
        if (pagetemp2.done)
        {
            printf("\nsecond request \n %s\n", buffer2);
            pagetemp2.done = false;
        }
        vTaskDelay(2000);
    };
}

int main()
{
    TaskHandle_t maintask;
    stdio_init_all();
    sleep_ms(10);
    xTaskCreate(mainTask, "maintask", 2048, NULL, 1, &maintask);
    vTaskStartScheduler();
    return 0;
}
