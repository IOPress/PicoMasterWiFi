#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "setupWifi.h"
#include "pico/aon_timer.h"
#include "time.h"
#include "lwip/dns.h"

struct timeStatus
{
    bool ready;
    struct udp_pcb *pcb;
};

void recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    struct timeStatus *tstatus = (struct timeStatus *)arg;
    printf("SNTP responded\n");
    if (p != NULL)
    {
        uint8_t seconds_buf[4];
        pbuf_copy_partial(p, seconds_buf, sizeof(seconds_buf), 40);
        uint32_t seconds_since_1900 = seconds_buf[0] << 24 | seconds_buf[1] << 16 | seconds_buf[2] << 8 | seconds_buf[3];
        time_t seconds_since_1970 = seconds_since_1900 - 2208988800;
        struct tm *datetime = gmtime(&seconds_since_1970);
        aon_timer_start_calendar(datetime);
        pbuf_free(p);
        udp_remove(pcb);
        tstatus->pcb=NULL;
        tstatus->ready = true;
    }
}

bool pollSNTP(struct timeStatus *tstatus)
{
    if (tstatus == NULL)
        return true;
    if (tstatus->ready)
    {
        free(tstatus);
        tstatus = NULL;
        return true;
    }
    return false;
}

void dns_found(const char *name, const ip_addr_t *ip, void *arg)
{
    struct timeStatus *tstatus = (struct timeStatus *)arg;
    printf("DNS %s\n", ipaddr_ntoa(ip));
    struct udp_pcb *pcb = udp_new();
    tstatus->pcb = pcb;
    udp_recv(pcb, recv, arg);
    udp_bind(pcb, IP_ADDR_ANY, 123);
    udp_connect(pcb, ip, 123);
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, 48, PBUF_RAM);
    uint8_t *payload = (uint8_t *)p->payload;
    memset(payload, 0, 48);
    *payload = 0x1b;
    err_t er = udp_send(pcb, p);
    pbuf_free(p);
}

struct timeStatus *getSNTP()
{
    struct timeStatus *tstatus = malloc(sizeof(struct timeStatus));
    tstatus->ready = false;
    tstatus->pcb=NULL;
    ip_addr_t ip;
    cyw43_arch_lwip_begin();
    err_t err = dns_gethostbyname("time.nist.gov", &ip, dns_found, tstatus);
    cyw43_arch_lwip_end();
    if (err == ERR_OK)
    {
        printf("DNS cache %s\n", ipaddr_ntoa(&ip));
        dns_found("", &ip, tstatus);
    }
    return tstatus;
}

void cancelSNTP(struct timeStatus *tstatus)
{
    if (tstatus != NULL)
    {
        udp_remove(tstatus->pcb);
        free(tstatus);
        tstatus = NULL;
        printf("canceled\n");
    }
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

    while (true)
    {
        struct timeStatus *tstatus = getSNTP();
        sleep_ms(500);
        if (pollSNTP(tstatus))
            break;
        cancelSNTP(tstatus);
    }

    while (true)
    {
        struct tm t;
        aon_timer_get_time_calendar(&t);
        char Date[100];
        strftime(Date, sizeof(Date), "Date: %a, %d %b %Y %k:%M:%S %Z\r\n", &t);
        printf("%s\n", Date);
        sleep_ms(5000);
    }
}
