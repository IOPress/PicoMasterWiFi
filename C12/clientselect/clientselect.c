
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include <FreeRTOS.h>
#include <task.h>
#include "pico/lwip_freertos.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "setupWifi.h"

void socketReadTimeout(int sockfd, char buffer[], int len, int timeoutsec)
{
    int m = 0;
    fd_set rfds;
    struct timeval tv;
    do
    {
        FD_ZERO(&rfds);
        FD_SET(sockfd, &rfds);
        tv.tv_sec = timeoutsec;
        tv.tv_usec = 0;
        int s = select(sockfd + 1, &rfds, NULL, NULL, &tv);
        if (s < 0)
        {
            printf("read error %X\n", errno);
            break;
        }
        if (s == 0)
        {
            printf("read timeout\n");
            break;
        }
        int n = read(sockfd, buffer + m, len - m - 1);
        if (n >= 0)
        {
            m = m + n;
            buffer[m] = 0;
            printf("\ndata received %d\n\n", n);
        }
        if (n < 0)
        {
            if (errno == ENOTCONN)
            {
                printf("socket closed by server\n");
                break;
            }
            printf("read error %X\n", errno);
            break;
        }

    } while (true);
}

void readdata(void *arg)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = inet_addr("23.192.228.80");

    connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));

    char header[] = "GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n ";
    int n = write(sockfd, header, strlen(header));
    printf("data sent \n");
    
    int len = 1024 * 2;
    char buffer[len];
    socketReadTimeout(sockfd, buffer, len,2);
    printf("Final buffer\n\n%s\n", buffer);

    printf("close socket %d\n", sockfd);
    close(sockfd);
    vTaskDelete(NULL);
}

void mainTask(void *arg)
{
    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
    }
    connectWiFi();
    xTaskCreate(readdata, "readdata", 2048, NULL, 1, NULL);
    while (true)
    {
        printf("doing other work");
        vTaskDelay(5000);
    }
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
