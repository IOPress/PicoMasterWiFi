#include <stdio.h>
#include "pico/stdlib.h"

#include "mbedtls/cipher.h"

#include "pico/rand.h"

int main()
{
    stdio_init_all();
    int ret;

    mbedtls_cipher_context_t cipher_ctx;
    mbedtls_cipher_init(&cipher_ctx);
    const mbedtls_cipher_info_t *cipher_info;
    cipher_info = mbedtls_cipher_info_from_string("AES-128-CBC");
    ret = mbedtls_cipher_setup(&cipher_ctx, cipher_info);

    unsigned char IV[16];
    get_rand_128((rng_128_t *)IV);
    ret = mbedtls_cipher_set_iv(&cipher_ctx, IV, 16);

    unsigned char key[16];
    get_rand_128((rng_128_t *)key);

    ret = mbedtls_cipher_setkey(&cipher_ctx, key,
                                cipher_info->key_bitlen, MBEDTLS_ENCRYPT);
    ret = mbedtls_cipher_reset(&cipher_ctx);

    char buffer[16] = "Hello World";
    char output[16];
    int olen;
    ret = mbedtls_cipher_update(&cipher_ctx, buffer, 16,
                                output, &olen);

    printf("cipher text ");
    for (int i = 0; i < olen; i++)
    {
        printf("%02X", output[i]);
    }
    printf("\n");

    char plaintext[16];
    ret = mbedtls_cipher_setkey(&cipher_ctx, key,
                                cipher_info->key_bitlen, MBEDTLS_DECRYPT);
    printf("%d\n", ret);
    ret = mbedtls_cipher_set_iv(&cipher_ctx, IV, 16);
    ret = mbedtls_cipher_reset(&cipher_ctx);
    ret = mbedtls_cipher_update(&cipher_ctx, output, 16,
                                plaintext, &olen);
    printf("plain text %.16s\n", plaintext);
    return 0;
}
