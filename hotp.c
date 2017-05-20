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

#include <stdio.h>
#include "hotp.h"
#include "hmac_sha1.h"

/**
 * Fast implementation of an integer power function.  Does not work with
 * negative integers.
 */
int ipow(int base, int exp)
{
    int result;

    result = 1;
    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        base *= base;
    }

    return result;
}

/**
 *
 *
 * @param[in]  counter    Counter value to generate a one-time password value
 *                        from.
 * @param[in]  digit_amt  Amount of digits to use in the one-time password.
 * @param[in]  key        Key material to use.  For optimal security, please
 *                        ensure that the key is >= sizeof(hash_digest) - in
 *                        this case 20.  Keys that are larger than BLOCKSIZE
 *                        will be hashed.
 * @param[in]  key_len    Size of key material.
 * @param[out] digits_out HOTP digit output.
 * @return                Negative value on failure, 0 on success.
 */
int hotp(uint64_t counter, int digit_amt,
         const uint8_t* key, size_t key_len,
         hotp_digits* digits_out)
{
    int rc, i, offset, value;
    uint8_t counter_enc[sizeof(counter)];
    uint8_t hmac[20];

    /* Sanity check digit count */
    if (digit_amt < MIN_DIGITS || digit_amt > MAX_DIGITS) {
        printf("Number of digits to calc must be %i <= out_digits <= %i.\n",
               MIN_DIGITS, MAX_DIGITS);
        return -1;
    }

    /* Encode counter bytes backwards into a byte array. This makes them read
       in sequence when passed into hmac_sha1. */
    for (i=sizeof(counter_enc)-1; i >= 0; i--) {
        counter_enc[i] = (uint8_t)(counter & 0xFF);
        counter >>= 8;
    }

    /* Calc HMAC of key and the encoded counter */
    rc = hmac_sha1(key, key_len, counter_enc, sizeof(counter_enc), hmac);
    if (rc < 0) {
        return rc;
    }

    /* Perform the "Dynamic Truncation" operation.  First we create an offset
       from the bottom 4 bits of the hmac calculated above, and then we use
       that offset on the hmac to create the P value (named `value` here
       because P is a terrible variable name.  Note the 0x7f is used to mask
       a potential set sign bit. */
    offset = hmac[sizeof(hmac)-1] & 0x0f;
    value = (hmac[offset] & 0x7f) << 24 |
            (hmac[offset+1] & 0xff) << 16 |
            (hmac[offset+2] & 0xff) << 8 |
            (hmac[offset+3] & 0xff);

    /* Get the otp digits from the P value.  The first module isolates the
       last `digit_amt` digits, and the second bit of modulo extracts each
       digit from the value. */
    value = value % ipow(10, digit_amt+1);
    for (i=digit_amt-1; i >= 0; i--) {
        digits_out->digits[i] = value % 10;
        value = value / 10;
    }
    digits_out->digit_amt = digit_amt;

    return 0;
}
