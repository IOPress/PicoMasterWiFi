
/* Workaround for some mbedtls source files using INT_MAX without including limits.h */
#include <limits.h>

// Hardware config
#define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_ENTROPY_HARDWARE_ALT
#define MBEDTLS_HAVE_TIME

// error reporting
#define MBEDTLS_ERROR_C

// used by lwIP
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_CTR_DRBG_C

// used by mbedtls
#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_ASN1_WRITE_C

// KEY EXCHANGE
#define MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
#define MBEDTLS_ECDH_C
#define MBEDTLS_ECDSA_C
#define MBEDTLS_ECP_C
#define MBEDTLS_PK_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_PK_PARSE_C

// Key exchange elliptic curves
#define MBEDTLS_ECP_DP_SECP192R1_ENABLED
#define MBEDTLS_ECP_DP_SECP224R1_ENABLED
#define MBEDTLS_ECP_DP_SECP256R1_ENABLED
#define MBEDTLS_ECP_DP_SECP384R1_ENABLED
#define MBEDTLS_ECP_DP_SECP521R1_ENABLED
#define MBEDTLS_ECP_DP_SECP192K1_ENABLED
#define MBEDTLS_ECP_DP_SECP224K1_ENABLED
#define MBEDTLS_ECP_DP_SECP256K1_ENABLED
#define MBEDTLS_ECP_DP_BP256R1_ENABLED
#define MBEDTLS_ECP_DP_BP384R1_ENABLED
#define MBEDTLS_ECP_DP_BP512R1_ENABLED
#define MBEDTLS_ECP_DP_CURVE25519_ENABLED


// symmetric encryption methods
#define MBEDTLS_CIPHER_C
#define MBEDTLS_AES_C
#define MBEDTLS_CCM_C
#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_AES_FEWER_TABLES
#define MBEDTLS_GCM_C
#define MBEDTLS_PK_PARSE_C

// hash and digest methods
#define MBEDTLS_MD_C
#define MBEDTLS_SHA224_C
#define MBEDTLS_SHA256_C
#define MBEDTLS_SHA384_C
#define MBEDTLS_SHA512_C



// certs
#define MBEDTLS_X509_CRT_PARSE_C
#define MBEDTLS_X509_USE_C
#define MBEDTLS_OID_C

// TLS
#define MBEDTLS_SSL_CLI_C
#define MBEDTLS_SSL_TLS_C
#define MBEDTLS_SSL_SERVER_NAME_INDICATION
#define MBEDTLS_SSL_PROTO_TLS1_2

#define MBEDTLS_SSL_SRV_C

// Speeds up mbedtls with NIST optimizations.
#define MBEDTLS_ECP_NIST_OPTIM




// checks that everything needed is included
#include "/home/mike/.pico-sdk/sdk/2.1.1/lib/mbedtls/include/mbedtls/check_config.h"
