/*
 *  This file is part of the luftschleuse2 project.
 *
 *  See https://github.com/muccc/luftschleuse2 for more information.
 *
 *  Copyright (C) 2013 Tobias Schneider <schneider@muc.ccc.de> 
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "aes/aes.h"
#include <avr/io.h>
#include <stdint.h>
#if 0
static const uint8_t aes_key[16]  = { 0x0, 0x0, 0x0, 0x0,
                   0x0, 0x0, 0x0, 0x0,
                   0x0, 0x0, 0x0, 0x0,
                   0x0, 0x0, 0x0, 0x0 };
#endif

#include "door-config.h"
static const uint8_t aes_key[16]  = {AES_KEY};

static aes128_ctx_t aes_ctx; /* the context where the round keys are stored */

void aes_handler_init(void)
{
    aes128_init(aes_key, &aes_ctx); /* generating the round keys from the 128 bit key */
}

void aes_encrypt(uint8_t *block)
{
    aes128_enc(block, &aes_ctx);
}

void aes_decrypt(uint8_t *block)
{
    aes128_dec(block, &aes_ctx);
}

