/*****************************************************************************\
Copyright (C) 2017 Nyxxie <nyxxxxie@gmail.com>
This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA  02110-1301, USA.
\*****************************************************************************/

#include <string.h>
#include "hmac_sha1.h"
#include "sha1.h"

/* Some constants the HMAC algorithm described in RFC 2104 defines. */
#define IPAD 0x36
#define OPAD 0x5C
#define BLOCK_SIZE 64

/* TODO: replace this with value from my sha1 implementation (current one
         doesn't define this). */
#define DIGEST_SIZE 20

/**
 * Implementation of the HMAC algorithm as described in RFC 2104, with SHA1
 * used for the hashing algorithm.
 *
 * @param[in]  key       Key material to use.  For optimal security, please
 *                       ensure that the key is >= sizeof(hash_digest) - in
 *                       this case 20.  Keys that are larger than BLOCKSIZE
 *                       will be hashed.
 * @param[in]  key_len   Size of key material.
 * @param[in]  text      Input data to be hashed.
 * @param[in]  text_len  Size of input data.
 * @param[out] hmac_out  Buffer to put calculated HMAC in.  Buffer should be
 *                       at least sizeof(hash_digest) bytes - again, 20 in
 *                       this case.
 * @return               Negative value on failure, 0 on success.
 */
int hmac_sha1(const uint8_t* key, size_t key_len,
              const uint8_t* text, size_t text_len,
              uint8_t* hmac_out)
{
    int i;
    SHA1_CTX ctx;
    uint8_t tmp[DIGEST_SIZE];
    uint8_t inner_block[BLOCK_SIZE];
    uint8_t outer_block[BLOCK_SIZE];

    /* If keylen > BLOCKSIZE, sha1 the key to make it fit in block */
    if (key_len > BLOCK_SIZE) {
        /* Hash key */
        SHA1_Init(&ctx);
        SHA1_Update(&ctx, key, key_len);
        SHA1_Final(tmp, &ctx);

        /* Continue execution using tmp as though it were key */
        key = key;
        key_len = DIGEST_SIZE;
    }

    /* Fill each block with it's cooresponding padding.  We can skip filling it
       with 0's as indicated in the spec since 0 XOR x = x. */
    for (i=0; i < BLOCK_SIZE; i++) {
        inner_block[i] = IPAD;
        outer_block[i] = OPAD;
    }

    /* XOR the key into each block */
    for (i=0; i < key_len; i++) {
        inner_block[i] ^= key[i];
        outer_block[i] ^= key[i];
    }

    /* Process inner hash.  Note that we get rid of the step where we append
       innerblock to text here by simply hashing each back to back. */
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, inner_block, sizeof(inner_block));
    SHA1_Update(&ctx, text, text_len);
    SHA1_Final(tmp, &ctx);

    /* Process outer hash.  We perform the same optimization as above here. */
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, outer_block, sizeof(outer_block));
    SHA1_Update(&ctx, tmp, sizeof(tmp));
    SHA1_Final(hmac_out, &ctx);

    return 0;
}
