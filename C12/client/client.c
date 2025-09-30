
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include <FreeRTOS.h>
#include <task.h>
#include "pico/lwip_freertos.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "setupWifi.h"

void mainTask(void *arg)
{    
    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
    }
    connectWiFi();

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = inet_addr("23.192.228.80");

    connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));

    char header[] = "GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n ";
    int n = write(sockfd, header, strlen(header));
    printf("data sent \n");
    char buffer[2048];
    int m=0;
    do
    {
        int n = read(sockfd, buffer+m, 2048-m);
        if(n<=0)break;
        buffer[n+m] = 0;
        printf("\ndata received %d\n\n", n);
        printf("%s\n\n", buffer+m);
        m=m+n;
    } while(true);
    printf("Final buffer\n\n%s\n", buffer);
    while (true)
    {
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
