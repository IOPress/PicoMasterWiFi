#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/ip_addr.h"
#include "setupWifi.h"

#include "lwip/apps/mqtt.h"

int mqttStatus = 0;

void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    if (status == MQTT_CONNECT_ACCEPTED)
    {
        printf("Accepted");
        mqttStatus = 1;
    }
}

static void pub_request_cb(void *arg, err_t result)
{
    printf("Publish result: %d\n", result);
    mqtt_client_t *client = (mqtt_client_t *)arg;
}

void sub_request_cb(void *arg, err_t result)
{
    printf("Subscribe result: %d\n", result);
    mqttStatus = 2;
}

static void incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
    printf("Topic %s. %d\n", topic, tot_len);
}

static void incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    char *payload = (char *)data;
    printf("payload\n%.*s\n", len, payload);
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
    
    mqtt_client_t *client = mqtt_client_new();
    struct mqtt_connect_client_info_t ci;
    memset(&ci, 0, sizeof(ci));
    ci.client_id = "MyPicoW";
    ci.keep_alive = 10;

    ip_addr_t ip;
    IP4_ADDR(&ip, 5, 196, 78, 28);
    // IP4_ADDR(&ip, 20, 79, 70, 109);

    err_t err = mqtt_client_connect(client, &ip, 1883, mqtt_connection_cb, 0, &ci);

    char payload[] = "Hello MQTT World";
    u8_t qos = 2;
    u8_t retain = 0;
    while (true)
    {
        switch (mqttStatus)
        {
        case 0:
            break;
        case 1:
            mqtt_set_inpub_callback(client, incoming_publish_cb, incoming_data_cb, NULL);
            err = mqtt_subscribe(client, "MyTopic", 1, sub_request_cb, NULL);
            break;

        case 2:
            err = mqtt_publish(client, "MyTopic", payload, strlen(payload), qos, retain, pub_request_cb, client);
            break;
        }
        sleep_ms(2000);
    }
}