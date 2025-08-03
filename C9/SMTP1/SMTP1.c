#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "setupWifi.h"
#include "lwip/apps/smtp.h"

void mailsent(void *arg, u8_t smtp_result, u16_t srv_err, err_t err)
{
    printf("mail (%p) sent with results: 0x%02x, 0x%04x, 0x%08x\n", arg,
           smtp_result, srv_err, err);
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

    smtp_set_server_addr("serverurl");
    smtp_set_server_port(25);
    smtp_set_auth(NULL, NULL);
    smtp_send_mail("person1@server.info", "person2@server.info", "subject", "body", mailsent, NULL);
    while (true)
    {
        sleep_ms(10);
    }
}
