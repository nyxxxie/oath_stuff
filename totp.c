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
#include <time.h>
#include "totp.h"

/**
 * Generates HOTP value.
 *
 * @param[in]  digit_amt  Amount of digits to use in the one-time password.
 * @param[in]  key        Key material to use.  For optimal security, please
 *                        ensure that the key is >= sizeof(hash_digest) - in
 *                        this case 20.  Keys that are larger than BLOCKSIZE
 *                        will be hashed.
 * @param[in]  key_len    Size of key material.
 * @param[out] digits_out HOTP digit output.
 * @return                Negative value on failure, 0 on success.
 */
int totp(int digit_amt, const uint8_t* key, size_t key_len, hotp_digits* digits_out)
{
    time_t steps;

    /* Get current unix time and determine step count */
    steps = time(NULL);
    if (steps < 0) {
        return -1;
    }
    steps /= TOTP_DEFAULT_TIME_STEP;

    /* Calc hotp given the above time steps as the counter parameter */
    return hotp((uint64_t)steps, digit_amt, key, key_len, digits_out);
}
