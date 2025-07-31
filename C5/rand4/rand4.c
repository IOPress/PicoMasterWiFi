#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/rand.h"

int main()
{
    stdio_init_all();

    for (int i = 0; i < 2000; i++)
    {
        uint32_t r = get_rand_32();
        printf("%032b\n", r);
    }
}