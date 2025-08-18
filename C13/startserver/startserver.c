#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include <FreeRTOS.h>
#include <task.h>
#include "pico/lwip_freertos.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "dhcpserver.h"

dhcp_server_t dhcp_server;

int setupAP()
{
    const char *ap_name = "MyPicoWAP";
    cyw43_arch_enable_ap_mode(ap_name, NULL, CYW43_AUTH_OPEN);
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

    dhcp_server_init(&dhcp_server, &(netif_default->ip_addr), &(netif_default->netmask));

    return 0;
}

bool isGet(char buffer[])
{
    char *myPtr1 = strstr(buffer, "GET");
    return myPtr1;
}

bool isPOST(char buffer[])
{
    char *myPtr1 = strstr(buffer, "POST");
    return myPtr1;
}

void getFileName(char buffer[], char file[])
{
    char *myPtr1 = strstr(buffer, " ") + 1;
    char *myPtr2 = strstr(myPtr1, " ");
    int c = myPtr2 - myPtr1;
    strncpy(file, myPtr1, c);
    file[c] = 0;
}

int getField(char buffer[],char name[], char value[]){
      char *myPtr1 = strstr(buffer, name);+strlen(name);
      if(myPtr1==NULL) return -1;
      char *myPtr2 = strstr(myPtr1, "\r\n");
      if(myPtr2==NULL) return -2;
      int c = myPtr2 - myPtr1;
      strncpy(value, myPtr1, c);
      value[c ] = 0;
      return 0;
}

void sendPage(int client_fd,char html[]){
   char headers[] = "HTTP/1.1 200 OK\r\nContent-Type:text/html;charset=UTF-8\r\nServer:Pico\r\nContent-Length:";
   char data[2048] = {0};
    snprintf(data, sizeof data, "%s%d\r\n\r\n%s", headers, strlen(html), html);         
    int   n = write(client_fd, data, strlen(data));   
}

void sendForm(int client_fd){
    char html[] = "<!DOCTYPE html>"
    "<html><head>"
    "<meta http-equiv=""content-type"" content=""text/html"" />"
    "<title>Setup</title></head><body>"
    "<form name=""Form"" action=""/"" method=""post"" enctype=""text/plain"">"
    "<p>SSID of the AP</p>"
    "<input name=""SSID"" style=""width: 6cm; height: 1cm""></input>"
    "<p>Password </p>"
    "<input name=""Password"" style=""width: 6cm; height: 1cm""></input>"
    "</br></br>"
    "<button>Ok</button>"
    "</form></body></html>";
    sendPage(client_fd,html);       
}

void mainTask(void *arg)
{
    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
    }
    setupAP();

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    listen(sockfd, 3);

    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof client_addr;
    char buffer[2048];
    while (true)
    {
        int client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
        int n = read(client_fd, buffer, 2048);
        buffer[n] = 0;    
        if (isGet(buffer))
        {
            // get file name
            char file[50] = {0};
            getFileName(buffer, file);
            if (strcmp(file, "/") == 0)
            {
               sendForm(client_fd);
            }
           
        }
        if (isPOST(buffer))
            {
                char ssid[50];
                getField(buffer,"SSID=", ssid);              
                char password[50];
                getField(buffer,"Password=", password);
                printf("ssid\n %s\n", ssid);
                printf("password\n %s\n", password);

                char html[] = "<html><head><title>Thanks</title></head><body><p>The new WiFi connection will now be made</p></body></html>\r\n";
                sendPage(client_fd,html);            
            }
        close(client_fd);
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