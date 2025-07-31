//Hardware config
#define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_ENTROPY_HARDWARE_ALT
#define MBEDTLS_HAVE_TIME
//error reporting
#define MBEDTLS_ERROR_C

//hash 
#define MBEDTLS_SHA256_C

#include "/home/mike/.pico-sdk/sdk/2.1.1/lib/mbedtls/include/mbedtls/check_config.h"