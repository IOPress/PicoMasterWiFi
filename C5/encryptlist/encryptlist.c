#include <stdio.h>
#include "pico/stdlib.h"

#include "mbedtls/cipher.h"

#include "pico/rand.h"

int main()
{
    stdio_init_all();
    int ret;

    const mbedtls_cipher_info_t *cipher_info1;
    const int *list;
    printf("Available ciphers:\n");
    list = mbedtls_cipher_list();
    while (*list)
    {
        cipher_info1 = mbedtls_cipher_info_from_type(*list);
        printf("  %s\n", cipher_info1->name);
        list++;
    }    
    return 0;
}
