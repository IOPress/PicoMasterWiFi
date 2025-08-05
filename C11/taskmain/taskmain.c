#include <stdio.h>
#include "pico/stdlib.h"
#include <FreeRTOS.h>
#include <task.h>


void task0(void *arg)
{
    for (;;)
    {
        gpio_put(2, 0);
    }
}
void task1(void *arg)
{
    for (;;)
    {
        gpio_put(2, 1);
    }
}

void mainapp(void *arg)
{
    UBaseType_t uxCoreAffinityMask;
    gpio_init(2);
    gpio_set_dir(2, GPIO_OUT);
    sleep_ms(1);
    TaskHandle_t th0;
    xTaskCreate(task0, "task1", 2048, NULL, 0, &th0);
    uxCoreAffinityMask = 1 << 0;
    vTaskCoreAffinitySet(th0, uxCoreAffinityMask);
    TaskHandle_t th1;
    xTaskCreate(task1, "task2", 2048, NULL, 0, &th1);
    uxCoreAffinityMask = 1 << 1;
    vTaskCoreAffinitySet(th1, uxCoreAffinityMask);
    vTaskDelay(100);
    TaskStatus_t TaskStatus;
    vTaskGetInfo(th1, &TaskStatus, true, true);
    printf("%ld\n", TaskStatus.usStackHighWaterMark);
    vTaskDelete(NULL);
}

int main()
{
    stdio_init_all();
    TaskHandle_t mainApp;
    xTaskCreate(mainapp, "mainapp", 2048, NULL, 0, &mainApp);
    vTaskStartScheduler();
}