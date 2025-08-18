
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include <FreeRTOS.h>
#include <task.h>
#include "pico/lwip_freertos.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "setupWifi.h"

void asyncServePage(void *arg)
{
    int client_fd = (int)arg;

    char buffer[2048];
    int n = read(client_fd, buffer, 2048);
    buffer[n] = 0;
    printf("%s", buffer);

    char headers[] = "HTTP/1.1 200 OK\r\nContent-Type:text/html;charset=UTF-8\r\nServer:Pico\r\nContent-Length:";
    char html[] = "<html><head><title>Hello HTTP World</title></head><body><p>Hello HTTP World</p></body></html>\r\n";
    char data[2048] = {0};
    snprintf(data, sizeof data, "%s%d\r\n\r\n%s", headers, strlen(html), html);

    n = write(client_fd, data, strlen(data));
    printf("data sent \n");
    close(client_fd);
    vTaskDelete(NULL);
}

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
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    listen(sockfd, 3);
    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof client_addr;
    while (true)
    {
        int client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);

        xTaskCreate(asyncServePage, "asyncServePage", 2048, (void *)client_fd, 1, NULL);
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
