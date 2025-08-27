#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "setupWifi.h"
#include "lwip/altcp_tls.h"
#include "lwip/apps/smtp.h"

void mailsent(void *arg, u8_t smtp_result,
                                 u16_t srv_err, err_t err)
{
    printf("mail (%p) sent with results:0x%02x, 0x%04x, 0x%08x\n", arg,
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
    connectWiFi();
    struct altcp_tls_config *tls_config =
                          altcp_tls_create_config_client(NULL, 0);
    smtp_set_tls_config(tls_config);

    smtp_set_server_addr("smtp.gmail.com");
    smtp_set_server_port(465);
    smtp_set_auth("gmail address", "app password");

    smtp_send_mail("gmail address", "recipients address",
                               "subject", "body", mailsent, NULL);
    while (true)
    {
        sleep_ms(10);
    }
}
