#pragma GCC optimize("O0")
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/structs/rosc.h"

uint8_t randomByte()
{
    uint32_t random = 0;
    uint32_t bit1;
    uint32_t bit2;

    for (int k = 0; k < 8; k++)
    {
        while (true)
        {
            bit1 = rosc_hw->randombit;
            sleep_us(1);
            bit2 = rosc_hw->randombit;
            if (bit1 != bit2)
                break;
            sleep_us(1);
        }
        random = (random << 1) | bit1;
    }
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