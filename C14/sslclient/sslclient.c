
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/lwip_freertos.h"

#include "setupWifi.h"

#include "lwip/sockets.h"
#include "lwip/inet.h"

#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/debug.h"

int mbedtls_net_send(void *ctx, const unsigned char *buf, size_t len)
{
    return write(*(int *)ctx, buf, len);
}

int mbedtls_net_recv(void *ctx, unsigned char *buf, size_t len)
{
    return (int)read(*(int *)ctx, buf, len);
}

static void my_debug(void *ctx, int level,
                     const char *file, int line, const char *str)
{
    fprintf((FILE *)ctx, "%s", str);
    fflush((FILE *)ctx);
}

void mainTask(void *arg)
{

    int err = 0;
    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
    }
    connectWiFi();

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(443);
    addr.sin_addr.s_addr = inet_addr("23.192.228.80");
 
    err = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (err < 0)
    { printf("%d\n",err);
        printf("Socket error %X\n", errno);
        printf("%d\n",sockfd);
    }

    mbedtls_ssl_config conf;
    mbedtls_ssl_config_init(&conf);
    err = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);

    mbedtls_entropy_context entropy;
    mbedtls_entropy_init(&entropy);

    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ctr_drbg_init(&ctr_drbg);

    const char *pers = "https_client";
    err = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers));
    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);

    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_NONE);

    mbedtls_debug_set_threshold(2);
    mbedtls_ssl_conf_dbg( &conf, my_debug, stdout );

    mbedtls_ssl_context ssl;
    mbedtls_ssl_init(&ssl);
    err = mbedtls_ssl_setup(&ssl, &conf);

    mbedtls_ssl_set_bio(&ssl, &sockfd, mbedtls_net_send, mbedtls_net_recv, NULL);

    char header[] = "GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n ";
   
    int n = mbedtls_ssl_write(&ssl, header, strlen(header));

    char buffer[2048];
    do
    { 
        n = mbedtls_ssl_read(&ssl, buffer, 2048);
        buffer[n] = 0;
        printf("data received %d\n", n);
        printf("%s\n", buffer);
    } while (n > 0);

    close(sockfd);
    mbedtls_ssl_free(&ssl);
    mbedtls_ssl_config_free(&conf);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
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
