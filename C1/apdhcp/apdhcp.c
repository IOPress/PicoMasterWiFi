#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "dhcpserver.h"

int main()
{

    stdio_init_all();
    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
        return 1;
    }
    const char *ap_name = "MyPicoWAP";
    const char *password = "mypassword";

    cyw43_arch_enable_ap_mode(ap_name, password, CYW43_AUTH_WPA2_AES_PSK);
    ip_addr_t ip;
    IP4_ADDR(&ip, 192, 168, 11, 1);
    netif_set_ipaddr(netif_default, &ip);
    IP4_ADDR(&ip, 255, 255, 255, 0);
    netif_set_netmask(netif_default, &ip);
    IP4_ADDR(&ip, 192, 168, 11, 1);
    netif_set_gw(netif_default, &ip);

    printf("IP: %s\n", ip4addr_ntoa(&(netif_default->ip_addr)));
    printf("Mask: %s\n", ip4addr_ntoa(&(netif_default->netmask)));
    printf("Gateway: %s\n", ip4addr_ntoa(&(netif_default->gw)));
    
    dhcp_server_t dhcp_server;
    dhcp_server_init(&dhcp_server, &(netif_default->ip_addr),&(netif_default->netmask));
    for (;;)
    {
    }
    dhcp_server_deinit(&dhcp_server);
    return 0;
}