//Hardware config
#define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_ENTROPY_HARDWARE_ALT
#define MBEDTLS_HAVE_TIME
//error reporting
#define MBEDTLS_ERROR_C

//hash 
#define MBEDTLS_SHA256_C

//Needed for sdk 2.2 and 2.3 mbedtls 3.x
#define MBEDTLS_ALLOW_PRIVATE_ACCESS
#define MBEDTLS_PLATFORM_MS_TIME_ALT

#include "/home/mike/.pico-sdk/sdk/2.1.1/lib/mbedtls/include/mbedtls/check_config.h"
