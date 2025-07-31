#pragma GCC optimize ("O0")
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/structs/rosc.h"

uint8_t randomByte()
{
    uint32_t random = 0;
    for (int k = 0; k < 8; k++)
    {
        random = (random << 1) | rosc_hw->randombit;
    }
    volatile int test=rosc_hw->status;
    return (uint8_t)random;
}

int main()
{
    stdio_init_all();
    for (int i = 0; i < 8000; i++)
    {
        uint8_t r = randomByte();
        printf("%08b\n", r);
    }
}