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

int main()
{
    stdio_init_all();

    UBaseType_t uxCoreAffinityMask;
    stdio_init_all();
    gpio_init(2);
    gpio_set_dir(2, GPIO_OUT);
    sleep_ms(10);

    TaskHandle_t th0;
    xTaskCreate(task0, "task1", 2048, NULL, 1, &th0);
    uxCoreAffinityMask = 1 << 1;
    vTaskCoreAffinitySet(th0, uxCoreAffinityMask);
    TaskHandle_t th1;
    xTaskCreate(task1, "task2", 2048, NULL, 1, &th1);
    uxCoreAffinityMask = 1 << 1;
    vTaskCoreAffinitySet(th1, uxCoreAffinityMask);
    vTaskStartScheduler();
}