#ifndef PRNG_H_INCLUDED
#define PRNG_H_INCLUDED

#include "mbedtls/ctr_drbg.h"

#include <stdint.h>

#define PRNG_MAX_SEED_LEN 256
#define PRNG_MAX_NUM_BYTES_GEN (1024 * 1024)
#define PRNG_INTERNAL_BUF_SIZE 1024

struct prng_context {
    uint32_t totlen;    // 0 .. PRNG_MAX_NUM_BYTES_GEN
    uint32_t remlen;    // 0 .. PRNG_INTERNAL_BUF_SIZE
    uint8_t buf[PRNG_INTERNAL_BUF_SIZE];
    mbedtls_ctr_drbg_context ctr_drbg_ctx;
};

int prng_instantiate(struct prng_context *ctx, const void *prng_seed, uint32_t prng_seed_len);
int prng_generate(struct prng_context *ctx, uint8_t *out, uint32_t outlen);

#endif // PRNG_H_INCLUDED
