#include "prng.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

static int _dummy_f_entropy(void *p_entropy, unsigned char *out, size_t outlen) {

    // The 2nd argument of mbedtls_ctr_drbg_seed() is of type:
    //
    //      int (*)(void *, unsigned char *, size_t)
    //
    // This is just a dummy implementation because for our deterministic use
    // case we never access external randomness source.
    (void) p_entropy;
    (void) out;

    return (outlen != 0) ? -1 : 0;
}

int prng_instantiate(struct prng_context *ctx, const void *seed, uint32_t seedlen) {

    // reject bad input
    if (!(ctx && (seedlen == 0 || (seed && seedlen <= PRNG_MAX_SEED_LEN)))) return -1;

    // totlen, remlen, buf
    memset(ctx, 0, sizeof (struct prng_context));

    // ctr_drbg_ctx
    mbedtls_ctr_drbg_init(&ctx->ctr_drbg_ctx);
    int err = mbedtls_ctr_drbg_seed(&ctx->ctr_drbg_ctx, _dummy_f_entropy, 0, seed, seedlen);
    if (err) return -2;

    // report success
    return 0;
}

int prng_generate(struct prng_context *ctx, uint8_t *out, uint32_t outlen) {

    // reject bad input
    if (!(
        ctx
        && ctx->totlen <= PRNG_MAX_NUM_BYTES_GEN
        && ctx->remlen <= PRNG_INTERNAL_BUF_SIZE
        && (outlen == 0 || out)
        && outlen < PRNG_MAX_NUM_BYTES_GEN
        && outlen + ctx->totlen < PRNG_MAX_NUM_BYTES_GEN
    )) return -1;

    while (outlen) {
        // ask CTR DRBG for more bytes, if no remaining bytes in the internal buffer
        if (ctx->remlen == 0) {
            int err = mbedtls_ctr_drbg_random(&ctx->ctr_drbg_ctx, ctx->buf, PRNG_INTERNAL_BUF_SIZE);
            if (err) return -2;
            ctx->remlen = PRNG_INTERNAL_BUF_SIZE;
        }

        // read nb random bytes from internal buffer
        uint32_t nb = outlen > ctx->remlen ? ctx->remlen : outlen;
        memcpy(out, &ctx->buf[PRNG_INTERNAL_BUF_SIZE - ctx->remlen], nb);

        // update pointers and counters to reflect that we just read nb bytes
        out += nb;
        outlen -= nb;
        ctx->totlen += nb;
        ctx->remlen -= nb;
    }

    return 0;
}
