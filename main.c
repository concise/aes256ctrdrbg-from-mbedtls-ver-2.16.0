#include "prng.h"

#include <stdio.h>

#define ERR_IF_NONZERO(err)     \
    do {                        \
        if (err) {              \
            printf("detect nonzero error code at line %d\n", __LINE__); \
            return 1;           \
        }                       \
    } while (0)

int main(void) {

    // below we generate a 2500-byte pseudorandom byte string from a 64-byte seed

    const void *const SEED = "\x61\xf3\x2c\xd3\x08\x31\x17\xab\x3a\xd0\x67\xbf\x12\x5c\xed\x76\x4a\x28\x80\xc4\x0a\xa1\x9b\x9e\x8d\x2c\xde\xbf\x27\xfb\xe6\xed\xec\xc2\x9d\xe5\x7e\x69\x78\xc5\xe9\xd6\x25\xdb\xca\x7f\x0e\x6d\x11\xbd\xcb\xd7\xf2\x79\x9f\xf0\x56\x5d\x2e\x8d\x19\x87\x32\x1a";

    const uint32_t SEEDLEN = 64;

    {
        struct prng_context my_prng_ctx = {0};
        unsigned char output[2500] = {0};
        int err = 0;
        err = prng_instantiate(&my_prng_ctx, SEED, SEEDLEN);
        ERR_IF_NONZERO(err);

        err = prng_generate(&my_prng_ctx, output, 2500);
        ERR_IF_NONZERO(err);

        for (int i = 0; i < 2500; ++i) printf("%02x", output[i]); printf("\n");
    }

    {
        struct prng_context my_prng_ctx = {0};
        unsigned char output[2500] = {0};
        int err = 0;
        err = prng_instantiate(&my_prng_ctx, SEED, SEEDLEN);
        ERR_IF_NONZERO(err);

        err = prng_generate(&my_prng_ctx, output, 7);
        ERR_IF_NONZERO(err);
        err = prng_generate(&my_prng_ctx, output + 7, 25);
        ERR_IF_NONZERO(err);
        err = prng_generate(&my_prng_ctx, output + 32, 2016);
        ERR_IF_NONZERO(err);
        err = prng_generate(&my_prng_ctx, output + 2048, 452);
        ERR_IF_NONZERO(err);


        for (int i = 0; i < 2500; ++i) printf("%02x", output[i]); printf("\n");
    }

    {
        struct prng_context my_prng_ctx = {0};
        unsigned char output[2500] = {0};
        int err = 0;
        err = prng_instantiate(&my_prng_ctx, SEED, SEEDLEN);
        ERR_IF_NONZERO(err);

        err = prng_generate(&my_prng_ctx, output, 1023);
        ERR_IF_NONZERO(err);
        err = prng_generate(&my_prng_ctx, output + 1023, 1);
        ERR_IF_NONZERO(err);
        err = prng_generate(&my_prng_ctx, output + 1024, 1023);
        ERR_IF_NONZERO(err);
        err = prng_generate(&my_prng_ctx, output + 2047, 453);
        ERR_IF_NONZERO(err);

        for (int i = 0; i < 2500; ++i) printf("%02x", output[i]); printf("\n");
    }

    return 0;
}
