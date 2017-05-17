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

#include "hotp.h"
#include "hmac_sha1.h"

struct hotp_digits {

};

// https://www.ietf.org/rfc/rfc4226.txt

int hotp(int number_digits
         const uint8_t* key, size_t key_len,
         const uint8_t* text, size_t text_len,
         hotp_digits* hotp)
{
    int rc;
    uint8_t hmac[20];

    /* Calc HMAC */
    rc = hmac_sha1(key, key_len, text, text_len, hmac);
    if (rc < 0) {
        return rc;
    }

    return 0;
}
