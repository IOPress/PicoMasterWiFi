#pragma GCC optimize("O0")
#include <stdio.h>
#include "pico/stdlib.h"
#include <FreeRTOS.h>
#include <task.h>
#include "semphr.h"

SemaphoreHandle_t xMutex;
bool done0=false;
bool done1=false;

uint64_t count = 0;

void task0(void *arg)
{

    for (int i = 0; i < 0xFFFFF; i++)
    {
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
        {
            count = count + 1;
            xSemaphoreGive(xMutex);
        }
    }
 done0=true;
    for (;;)
    {
    }
}
void task1(void *arg)
{

    for (int i = 0; i < 0xFFFFF; i++)
    {
     if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
      {
            count = count + 1;
            xSemaphoreGive(xMutex);
       }
    }
 done1=true;
    for (;;)
    {
    }
}

void task2(void *arg)
{
    printf("start\n");
while(!done0 || !done1 ){};    printf("%llX\n", count);
    fflush(stdout);
    for (;;)
    {
    }
}


int main()
{

     stdio_init_all();
     sleep_ms(10);

    UBaseType_t uxCoreAffinityMask;
   
    TaskHandle_t th0;
    xTaskCreate(task0, "task0", 2048, NULL, 1, &th0);
    uxCoreAffinityMask = 1 << 0;
    vTaskCoreAffinitySet(th0, uxCoreAffinityMask);

    TaskHandle_t th1;
    xTaskCreate(task1, "task1", 2048, NULL, 1, &th1);
    uxCoreAffinityMask = 1 << 1;
    vTaskCoreAffinitySet(th1, uxCoreAffinityMask);

    TaskHandle_t th2;
    xTaskCreate(task2, "task2", 2048, NULL, 1, &th2);
    uxCoreAffinityMask = 1 << 1;
    vTaskCoreAffinitySet(th2, uxCoreAffinityMask);

    xMutex = xSemaphoreCreateMutex();

    vTaskStartScheduler();
}
