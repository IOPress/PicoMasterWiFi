#include <stdio.h>
#include "pico/stdlib.h"
#include <string.h>
#include "mbedtls/sha256.h"

int main()
{
    stdio_init_all();

    char input[] = "Hello SHA256 World";
    unsigned char hash[32];
    int ret;

    mbedtls_sha256_context ctx;
    mbedtls_sha256_init(&ctx);
    ret = mbedtls_sha256_starts_ret(&ctx, 0);

    ret = mbedtls_sha256_update_ret(&ctx, input, strlen(input));
    ret = mbedtls_sha256_finish_ret(&ctx, hash);
    ret = mbedtls_sha256_ret(input, strlen(input), hash, 0);
    printf("Input string: \"%s\"\n", input);
    printf("SHA256 Hash: ");
    for (int i = 0; i < 32; i++)
    {
        printf("%02X", hash[i]);
    }
    printf("\n");
    mbedtls_sha256_free(&ctx);
    return 0;
}
