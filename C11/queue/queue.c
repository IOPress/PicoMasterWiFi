#pragma GCC optimize("O0")
#include <stdio.h>
#include "pico/stdlib.h"
#include <FreeRTOS.h>
#include <task.h>
#include "semphr.h"

QueueHandle_t q;
int64_t count = 0;

void task0(void* arg) {
    for (;;) {
        xQueueSendToBack(q, &count, 2);
        count++;
        vTaskDelay(1);
    }
}
void task1(void* arg) {
    int64_t data;
    for (;;) {
        xQueueReceive(q, &data, 20);
        printf("%llX %d\n", data,uxQueueSpacesAvailable(q));
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
    q = xQueueCreate(100, sizeof(int64_t));
    vTaskStartScheduler();
}
