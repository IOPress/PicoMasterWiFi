//Hardware config
#define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_ENTROPY_HARDWARE_ALT
#define MBEDTLS_HAVE_TIME
//error reporting
#define MBEDTLS_ERROR_C
//encryption
#define MBEDTLS_AES_C
#define MBEDTLS_CCM_C
#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_AES_FEWER_TABLES
#define MBEDTLS_CIPHER_C


#include "/home/mike/.pico-sdk/sdk/2.1.1/lib/mbedtls/include/mbedtls/check_config.h"