/*

 \file manchester.c

 \brief Fast manchester encode/decode

 \author Michael Ambrus

 */

#include <stdint.h>
#include "manchester.h"
#include <assert.h>
#include <stddef.h>

#define O 0x2                   /* (1, 0) */
#define I 0x1                   /* (0, 1) */

#define MCHW(B7, B6, B5, B4, B3, B2, B1, B0) \
    (B7 << 14) | \
    (B6 << 12) | \
    (B5 << 10) | \
    (B4 << 8)  | \
    (B3 << 6)  | \
    (B2 << 4)  | \
    (B1 << 2)  | \
    (B0 << 0)

static const uint32_t table[] = {
    MCHW(O, O, O, O, O, O, O, O),
    MCHW(O, O, O, O, O, O, O, I),
    MCHW(O, O, O, O, O, O, I, O),
    MCHW(O, O, O, O, O, O, I, I),
    MCHW(O, O, O, O, O, I, O, O),
    MCHW(O, O, O, O, O, I, O, I),
    MCHW(O, O, O, O, O, I, I, O),
    MCHW(O, O, O, O, O, I, I, I),
    MCHW(O, O, O, O, I, O, O, O),
    MCHW(O, O, O, O, I, O, O, I),
    MCHW(O, O, O, O, I, O, I, O),
    MCHW(O, O, O, O, I, O, I, I),
    MCHW(O, O, O, O, I, I, O, O),
    MCHW(O, O, O, O, I, I, O, I),
    MCHW(O, O, O, O, I, I, I, O),
    MCHW(O, O, O, O, I, I, I, I),
    MCHW(O, O, O, I, O, O, O, O),
    MCHW(O, O, O, I, O, O, O, I),
    MCHW(O, O, O, I, O, O, I, O),
    MCHW(O, O, O, I, O, O, I, I),
    MCHW(O, O, O, I, O, I, O, O),
    MCHW(O, O, O, I, O, I, O, I),
    MCHW(O, O, O, I, O, I, I, O),
    MCHW(O, O, O, I, O, I, I, I),
    MCHW(O, O, O, I, I, O, O, O),
    MCHW(O, O, O, I, I, O, O, I),
    MCHW(O, O, O, I, I, O, I, O),
    MCHW(O, O, O, I, I, O, I, I),
    MCHW(O, O, O, I, I, I, O, O),
    MCHW(O, O, O, I, I, I, O, I),
    MCHW(O, O, O, I, I, I, I, O),
    MCHW(O, O, O, I, I, I, I, I),
    MCHW(O, O, I, O, O, O, O, O),
    MCHW(O, O, I, O, O, O, O, I),
    MCHW(O, O, I, O, O, O, I, O),
    MCHW(O, O, I, O, O, O, I, I),
    MCHW(O, O, I, O, O, I, O, O),
    MCHW(O, O, I, O, O, I, O, I),
    MCHW(O, O, I, O, O, I, I, O),
    MCHW(O, O, I, O, O, I, I, I),
    MCHW(O, O, I, O, I, O, O, O),
    MCHW(O, O, I, O, I, O, O, I),
    MCHW(O, O, I, O, I, O, I, O),
    MCHW(O, O, I, O, I, O, I, I),
    MCHW(O, O, I, O, I, I, O, O),
    MCHW(O, O, I, O, I, I, O, I),
    MCHW(O, O, I, O, I, I, I, O),
    MCHW(O, O, I, O, I, I, I, I),
    MCHW(O, O, I, I, O, O, O, O),
    MCHW(O, O, I, I, O, O, O, I),
    MCHW(O, O, I, I, O, O, I, O),
    MCHW(O, O, I, I, O, O, I, I),
    MCHW(O, O, I, I, O, I, O, O),
    MCHW(O, O, I, I, O, I, O, I),
    MCHW(O, O, I, I, O, I, I, O),
    MCHW(O, O, I, I, O, I, I, I),
    MCHW(O, O, I, I, I, O, O, O),
    MCHW(O, O, I, I, I, O, O, I),
    MCHW(O, O, I, I, I, O, I, O),
    MCHW(O, O, I, I, I, O, I, I),
    MCHW(O, O, I, I, I, I, O, O),
    MCHW(O, O, I, I, I, I, O, I),
    MCHW(O, O, I, I, I, I, I, O),
    MCHW(O, O, I, I, I, I, I, I),
    MCHW(O, I, O, O, O, O, O, O),
    MCHW(O, I, O, O, O, O, O, I),
    MCHW(O, I, O, O, O, O, I, O),
    MCHW(O, I, O, O, O, O, I, I),
    MCHW(O, I, O, O, O, I, O, O),
    MCHW(O, I, O, O, O, I, O, I),
    MCHW(O, I, O, O, O, I, I, O),
    MCHW(O, I, O, O, O, I, I, I),
    MCHW(O, I, O, O, I, O, O, O),
    MCHW(O, I, O, O, I, O, O, I),
    MCHW(O, I, O, O, I, O, I, O),
    MCHW(O, I, O, O, I, O, I, I),
    MCHW(O, I, O, O, I, I, O, O),
    MCHW(O, I, O, O, I, I, O, I),
    MCHW(O, I, O, O, I, I, I, O),
    MCHW(O, I, O, O, I, I, I, I),
    MCHW(O, I, O, I, O, O, O, O),
    MCHW(O, I, O, I, O, O, O, I),
    MCHW(O, I, O, I, O, O, I, O),
    MCHW(O, I, O, I, O, O, I, I),
    MCHW(O, I, O, I, O, I, O, O),
    MCHW(O, I, O, I, O, I, O, I),
    MCHW(O, I, O, I, O, I, I, O),
    MCHW(O, I, O, I, O, I, I, I),
    MCHW(O, I, O, I, I, O, O, O),
    MCHW(O, I, O, I, I, O, O, I),
    MCHW(O, I, O, I, I, O, I, O),
    MCHW(O, I, O, I, I, O, I, I),
    MCHW(O, I, O, I, I, I, O, O),
    MCHW(O, I, O, I, I, I, O, I),
    MCHW(O, I, O, I, I, I, I, O),
    MCHW(O, I, O, I, I, I, I, I),
    MCHW(O, I, I, O, O, O, O, O),
    MCHW(O, I, I, O, O, O, O, I),
    MCHW(O, I, I, O, O, O, I, O),
    MCHW(O, I, I, O, O, O, I, I),
    MCHW(O, I, I, O, O, I, O, O),
    MCHW(O, I, I, O, O, I, O, I),
    MCHW(O, I, I, O, O, I, I, O),
    MCHW(O, I, I, O, O, I, I, I),
    MCHW(O, I, I, O, I, O, O, O),
    MCHW(O, I, I, O, I, O, O, I),
    MCHW(O, I, I, O, I, O, I, O),
    MCHW(O, I, I, O, I, O, I, I),
    MCHW(O, I, I, O, I, I, O, O),
    MCHW(O, I, I, O, I, I, O, I),
    MCHW(O, I, I, O, I, I, I, O),
    MCHW(O, I, I, O, I, I, I, I),
    MCHW(O, I, I, I, O, O, O, O),
    MCHW(O, I, I, I, O, O, O, I),
    MCHW(O, I, I, I, O, O, I, O),
    MCHW(O, I, I, I, O, O, I, I),
    MCHW(O, I, I, I, O, I, O, O),
    MCHW(O, I, I, I, O, I, O, I),
    MCHW(O, I, I, I, O, I, I, O),
    MCHW(O, I, I, I, O, I, I, I),
    MCHW(O, I, I, I, I, O, O, O),
    MCHW(O, I, I, I, I, O, O, I),
    MCHW(O, I, I, I, I, O, I, O),
    MCHW(O, I, I, I, I, O, I, I),
    MCHW(O, I, I, I, I, I, O, O),
    MCHW(O, I, I, I, I, I, O, I),
    MCHW(O, I, I, I, I, I, I, O),
    MCHW(O, I, I, I, I, I, I, I),
    MCHW(I, O, O, O, O, O, O, O),
    MCHW(I, O, O, O, O, O, O, I),
    MCHW(I, O, O, O, O, O, I, O),
    MCHW(I, O, O, O, O, O, I, I),
    MCHW(I, O, O, O, O, I, O, O),
    MCHW(I, O, O, O, O, I, O, I),
    MCHW(I, O, O, O, O, I, I, O),
    MCHW(I, O, O, O, O, I, I, I),
    MCHW(I, O, O, O, I, O, O, O),
    MCHW(I, O, O, O, I, O, O, I),
    MCHW(I, O, O, O, I, O, I, O),
    MCHW(I, O, O, O, I, O, I, I),
    MCHW(I, O, O, O, I, I, O, O),
    MCHW(I, O, O, O, I, I, O, I),
    MCHW(I, O, O, O, I, I, I, O),
    MCHW(I, O, O, O, I, I, I, I),
    MCHW(I, O, O, I, O, O, O, O),
    MCHW(I, O, O, I, O, O, O, I),
    MCHW(I, O, O, I, O, O, I, O),
    MCHW(I, O, O, I, O, O, I, I),
    MCHW(I, O, O, I, O, I, O, O),
    MCHW(I, O, O, I, O, I, O, I),
    MCHW(I, O, O, I, O, I, I, O),
    MCHW(I, O, O, I, O, I, I, I),
    MCHW(I, O, O, I, I, O, O, O),
    MCHW(I, O, O, I, I, O, O, I),
    MCHW(I, O, O, I, I, O, I, O),
    MCHW(I, O, O, I, I, O, I, I),
    MCHW(I, O, O, I, I, I, O, O),
    MCHW(I, O, O, I, I, I, O, I),
    MCHW(I, O, O, I, I, I, I, O),
    MCHW(I, O, O, I, I, I, I, I),
    MCHW(I, O, I, O, O, O, O, O),
    MCHW(I, O, I, O, O, O, O, I),
    MCHW(I, O, I, O, O, O, I, O),
    MCHW(I, O, I, O, O, O, I, I),
    MCHW(I, O, I, O, O, I, O, O),
    MCHW(I, O, I, O, O, I, O, I),
    MCHW(I, O, I, O, O, I, I, O),
    MCHW(I, O, I, O, O, I, I, I),
    MCHW(I, O, I, O, I, O, O, O),
    MCHW(I, O, I, O, I, O, O, I),
    MCHW(I, O, I, O, I, O, I, O),
    MCHW(I, O, I, O, I, O, I, I),
    MCHW(I, O, I, O, I, I, O, O),
    MCHW(I, O, I, O, I, I, O, I),
    MCHW(I, O, I, O, I, I, I, O),
    MCHW(I, O, I, O, I, I, I, I),
    MCHW(I, O, I, I, O, O, O, O),
    MCHW(I, O, I, I, O, O, O, I),
    MCHW(I, O, I, I, O, O, I, O),
    MCHW(I, O, I, I, O, O, I, I),
    MCHW(I, O, I, I, O, I, O, O),
    MCHW(I, O, I, I, O, I, O, I),
    MCHW(I, O, I, I, O, I, I, O),
    MCHW(I, O, I, I, O, I, I, I),
    MCHW(I, O, I, I, I, O, O, O),
    MCHW(I, O, I, I, I, O, O, I),
    MCHW(I, O, I, I, I, O, I, O),
    MCHW(I, O, I, I, I, O, I, I),
    MCHW(I, O, I, I, I, I, O, O),
    MCHW(I, O, I, I, I, I, O, I),
    MCHW(I, O, I, I, I, I, I, O),
    MCHW(I, O, I, I, I, I, I, I),
    MCHW(I, I, O, O, O, O, O, O),
    MCHW(I, I, O, O, O, O, O, I),
    MCHW(I, I, O, O, O, O, I, O),
    MCHW(I, I, O, O, O, O, I, I),
    MCHW(I, I, O, O, O, I, O, O),
    MCHW(I, I, O, O, O, I, O, I),
    MCHW(I, I, O, O, O, I, I, O),
    MCHW(I, I, O, O, O, I, I, I),
    MCHW(I, I, O, O, I, O, O, O),
    MCHW(I, I, O, O, I, O, O, I),
    MCHW(I, I, O, O, I, O, I, O),
    MCHW(I, I, O, O, I, O, I, I),
    MCHW(I, I, O, O, I, I, O, O),
    MCHW(I, I, O, O, I, I, O, I),
    MCHW(I, I, O, O, I, I, I, O),
    MCHW(I, I, O, O, I, I, I, I),
    MCHW(I, I, O, I, O, O, O, O),
    MCHW(I, I, O, I, O, O, O, I),
    MCHW(I, I, O, I, O, O, I, O),
    MCHW(I, I, O, I, O, O, I, I),
    MCHW(I, I, O, I, O, I, O, O),
    MCHW(I, I, O, I, O, I, O, I),
    MCHW(I, I, O, I, O, I, I, O),
    MCHW(I, I, O, I, O, I, I, I),
    MCHW(I, I, O, I, I, O, O, O),
    MCHW(I, I, O, I, I, O, O, I),
    MCHW(I, I, O, I, I, O, I, O),
    MCHW(I, I, O, I, I, O, I, I),
    MCHW(I, I, O, I, I, I, O, O),
    MCHW(I, I, O, I, I, I, O, I),
    MCHW(I, I, O, I, I, I, I, O),
    MCHW(I, I, O, I, I, I, I, I),
    MCHW(I, I, I, O, O, O, O, O),
    MCHW(I, I, I, O, O, O, O, I),
    MCHW(I, I, I, O, O, O, I, O),
    MCHW(I, I, I, O, O, O, I, I),
    MCHW(I, I, I, O, O, I, O, O),
    MCHW(I, I, I, O, O, I, O, I),
    MCHW(I, I, I, O, O, I, I, O),
    MCHW(I, I, I, O, O, I, I, I),
    MCHW(I, I, I, O, I, O, O, O),
    MCHW(I, I, I, O, I, O, O, I),
    MCHW(I, I, I, O, I, O, I, O),
    MCHW(I, I, I, O, I, O, I, I),
    MCHW(I, I, I, O, I, I, O, O),
    MCHW(I, I, I, O, I, I, O, I),
    MCHW(I, I, I, O, I, I, I, O),
    MCHW(I, I, I, O, I, I, I, I),
    MCHW(I, I, I, I, O, O, O, O),
    MCHW(I, I, I, I, O, O, O, I),
    MCHW(I, I, I, I, O, O, I, O),
    MCHW(I, I, I, I, O, O, I, I),
    MCHW(I, I, I, I, O, I, O, O),
    MCHW(I, I, I, I, O, I, O, I),
    MCHW(I, I, I, I, O, I, I, O),
    MCHW(I, I, I, I, O, I, I, I),
    MCHW(I, I, I, I, I, O, O, O),
    MCHW(I, I, I, I, I, O, O, I),
    MCHW(I, I, I, I, I, O, I, O),
    MCHW(I, I, I, I, I, O, I, I),
    MCHW(I, I, I, I, I, I, O, O),
    MCHW(I, I, I, I, I, I, O, I),
    MCHW(I, I, I, I, I, I, I, O),
    MCHW(I, I, I, I, I, I, I, I)
};

uint16_t manchester_encode(uint8_t i)
{
    return table[i];
};

uint8_t manchester_decode(uint16_t c)
{
    switch (c) {

        case MCHW(O, O, O, O, O, O, O, O):
            return 0;
            break;
        case MCHW(O, O, O, O, O, O, O, I):
            return 1;
            break;
        case MCHW(O, O, O, O, O, O, I, O):
            return 2;
            break;
        case MCHW(O, O, O, O, O, O, I, I):
            return 3;
            break;
        case MCHW(O, O, O, O, O, I, O, O):
            return 4;
            break;
        case MCHW(O, O, O, O, O, I, O, I):
            return 5;
            break;
        case MCHW(O, O, O, O, O, I, I, O):
            return 6;
            break;
        case MCHW(O, O, O, O, O, I, I, I):
            return 7;
            break;
        case MCHW(O, O, O, O, I, O, O, O):
            return 8;
            break;
        case MCHW(O, O, O, O, I, O, O, I):
            return 9;
            break;
        case MCHW(O, O, O, O, I, O, I, O):
            return 10;
            break;
        case MCHW(O, O, O, O, I, O, I, I):
            return 11;
            break;
        case MCHW(O, O, O, O, I, I, O, O):
            return 12;
            break;
        case MCHW(O, O, O, O, I, I, O, I):
            return 13;
            break;
        case MCHW(O, O, O, O, I, I, I, O):
            return 14;
            break;
        case MCHW(O, O, O, O, I, I, I, I):
            return 15;
            break;
        case MCHW(O, O, O, I, O, O, O, O):
            return 16;
            break;
        case MCHW(O, O, O, I, O, O, O, I):
            return 17;
            break;
        case MCHW(O, O, O, I, O, O, I, O):
            return 18;
            break;
        case MCHW(O, O, O, I, O, O, I, I):
            return 19;
            break;
        case MCHW(O, O, O, I, O, I, O, O):
            return 20;
            break;
        case MCHW(O, O, O, I, O, I, O, I):
            return 21;
            break;
        case MCHW(O, O, O, I, O, I, I, O):
            return 22;
            break;
        case MCHW(O, O, O, I, O, I, I, I):
            return 23;
            break;
        case MCHW(O, O, O, I, I, O, O, O):
            return 24;
            break;
        case MCHW(O, O, O, I, I, O, O, I):
            return 25;
            break;
        case MCHW(O, O, O, I, I, O, I, O):
            return 26;
            break;
        case MCHW(O, O, O, I, I, O, I, I):
            return 27;
            break;
        case MCHW(O, O, O, I, I, I, O, O):
            return 28;
            break;
        case MCHW(O, O, O, I, I, I, O, I):
            return 29;
            break;
        case MCHW(O, O, O, I, I, I, I, O):
            return 30;
            break;
        case MCHW(O, O, O, I, I, I, I, I):
            return 31;
            break;
        case MCHW(O, O, I, O, O, O, O, O):
            return 32;
            break;
        case MCHW(O, O, I, O, O, O, O, I):
            return 33;
            break;
        case MCHW(O, O, I, O, O, O, I, O):
            return 34;
            break;
        case MCHW(O, O, I, O, O, O, I, I):
            return 35;
            break;
        case MCHW(O, O, I, O, O, I, O, O):
            return 36;
            break;
        case MCHW(O, O, I, O, O, I, O, I):
            return 37;
            break;
        case MCHW(O, O, I, O, O, I, I, O):
            return 38;
            break;
        case MCHW(O, O, I, O, O, I, I, I):
            return 39;
            break;
        case MCHW(O, O, I, O, I, O, O, O):
            return 40;
            break;
        case MCHW(O, O, I, O, I, O, O, I):
            return 41;
            break;
        case MCHW(O, O, I, O, I, O, I, O):
            return 42;
            break;
        case MCHW(O, O, I, O, I, O, I, I):
            return 43;
            break;
        case MCHW(O, O, I, O, I, I, O, O):
            return 44;
            break;
        case MCHW(O, O, I, O, I, I, O, I):
            return 45;
            break;
        case MCHW(O, O, I, O, I, I, I, O):
            return 46;
            break;
        case MCHW(O, O, I, O, I, I, I, I):
            return 47;
            break;
        case MCHW(O, O, I, I, O, O, O, O):
            return 48;
            break;
        case MCHW(O, O, I, I, O, O, O, I):
            return 49;
            break;
        case MCHW(O, O, I, I, O, O, I, O):
            return 50;
            break;
        case MCHW(O, O, I, I, O, O, I, I):
            return 51;
            break;
        case MCHW(O, O, I, I, O, I, O, O):
            return 52;
            break;
        case MCHW(O, O, I, I, O, I, O, I):
            return 53;
            break;
        case MCHW(O, O, I, I, O, I, I, O):
            return 54;
            break;
        case MCHW(O, O, I, I, O, I, I, I):
            return 55;
            break;
        case MCHW(O, O, I, I, I, O, O, O):
            return 56;
            break;
        case MCHW(O, O, I, I, I, O, O, I):
            return 57;
            break;
        case MCHW(O, O, I, I, I, O, I, O):
            return 58;
            break;
        case MCHW(O, O, I, I, I, O, I, I):
            return 59;
            break;
        case MCHW(O, O, I, I, I, I, O, O):
            return 60;
            break;
        case MCHW(O, O, I, I, I, I, O, I):
            return 61;
            break;
        case MCHW(O, O, I, I, I, I, I, O):
            return 62;
            break;
        case MCHW(O, O, I, I, I, I, I, I):
            return 63;
            break;
        case MCHW(O, I, O, O, O, O, O, O):
            return 64;
            break;
        case MCHW(O, I, O, O, O, O, O, I):
            return 65;
            break;
        case MCHW(O, I, O, O, O, O, I, O):
            return 66;
            break;
        case MCHW(O, I, O, O, O, O, I, I):
            return 67;
            break;
        case MCHW(O, I, O, O, O, I, O, O):
            return 68;
            break;
        case MCHW(O, I, O, O, O, I, O, I):
            return 69;
            break;
        case MCHW(O, I, O, O, O, I, I, O):
            return 70;
            break;
        case MCHW(O, I, O, O, O, I, I, I):
            return 71;
            break;
        case MCHW(O, I, O, O, I, O, O, O):
            return 72;
            break;
        case MCHW(O, I, O, O, I, O, O, I):
            return 73;
            break;
        case MCHW(O, I, O, O, I, O, I, O):
            return 74;
            break;
        case MCHW(O, I, O, O, I, O, I, I):
            return 75;
            break;
        case MCHW(O, I, O, O, I, I, O, O):
            return 76;
            break;
        case MCHW(O, I, O, O, I, I, O, I):
            return 77;
            break;
        case MCHW(O, I, O, O, I, I, I, O):
            return 78;
            break;
        case MCHW(O, I, O, O, I, I, I, I):
            return 79;
            break;
        case MCHW(O, I, O, I, O, O, O, O):
            return 80;
            break;
        case MCHW(O, I, O, I, O, O, O, I):
            return 81;
            break;
        case MCHW(O, I, O, I, O, O, I, O):
            return 82;
            break;
        case MCHW(O, I, O, I, O, O, I, I):
            return 83;
            break;
        case MCHW(O, I, O, I, O, I, O, O):
            return 84;
            break;
        case MCHW(O, I, O, I, O, I, O, I):
            return 85;
            break;
        case MCHW(O, I, O, I, O, I, I, O):
            return 86;
            break;
        case MCHW(O, I, O, I, O, I, I, I):
            return 87;
            break;
        case MCHW(O, I, O, I, I, O, O, O):
            return 88;
            break;
        case MCHW(O, I, O, I, I, O, O, I):
            return 89;
            break;
        case MCHW(O, I, O, I, I, O, I, O):
            return 90;
            break;
        case MCHW(O, I, O, I, I, O, I, I):
            return 91;
            break;
        case MCHW(O, I, O, I, I, I, O, O):
            return 92;
            break;
        case MCHW(O, I, O, I, I, I, O, I):
            return 93;
            break;
        case MCHW(O, I, O, I, I, I, I, O):
            return 94;
            break;
        case MCHW(O, I, O, I, I, I, I, I):
            return 95;
            break;
        case MCHW(O, I, I, O, O, O, O, O):
            return 96;
            break;
        case MCHW(O, I, I, O, O, O, O, I):
            return 97;
            break;
        case MCHW(O, I, I, O, O, O, I, O):
            return 98;
            break;
        case MCHW(O, I, I, O, O, O, I, I):
            return 99;
            break;
        case MCHW(O, I, I, O, O, I, O, O):
            return 100;
            break;
        case MCHW(O, I, I, O, O, I, O, I):
            return 101;
            break;
        case MCHW(O, I, I, O, O, I, I, O):
            return 102;
            break;
        case MCHW(O, I, I, O, O, I, I, I):
            return 103;
            break;
        case MCHW(O, I, I, O, I, O, O, O):
            return 104;
            break;
        case MCHW(O, I, I, O, I, O, O, I):
            return 105;
            break;
        case MCHW(O, I, I, O, I, O, I, O):
            return 106;
            break;
        case MCHW(O, I, I, O, I, O, I, I):
            return 107;
            break;
        case MCHW(O, I, I, O, I, I, O, O):
            return 108;
            break;
        case MCHW(O, I, I, O, I, I, O, I):
            return 109;
            break;
        case MCHW(O, I, I, O, I, I, I, O):
            return 110;
            break;
        case MCHW(O, I, I, O, I, I, I, I):
            return 111;
            break;
        case MCHW(O, I, I, I, O, O, O, O):
            return 112;
            break;
        case MCHW(O, I, I, I, O, O, O, I):
            return 113;
            break;
        case MCHW(O, I, I, I, O, O, I, O):
            return 114;
            break;
        case MCHW(O, I, I, I, O, O, I, I):
            return 115;
            break;
        case MCHW(O, I, I, I, O, I, O, O):
            return 116;
            break;
        case MCHW(O, I, I, I, O, I, O, I):
            return 117;
            break;
        case MCHW(O, I, I, I, O, I, I, O):
            return 118;
            break;
        case MCHW(O, I, I, I, O, I, I, I):
            return 119;
            break;
        case MCHW(O, I, I, I, I, O, O, O):
            return 120;
            break;
        case MCHW(O, I, I, I, I, O, O, I):
            return 121;
            break;
        case MCHW(O, I, I, I, I, O, I, O):
            return 122;
            break;
        case MCHW(O, I, I, I, I, O, I, I):
            return 123;
            break;
        case MCHW(O, I, I, I, I, I, O, O):
            return 124;
            break;
        case MCHW(O, I, I, I, I, I, O, I):
            return 125;
            break;
        case MCHW(O, I, I, I, I, I, I, O):
            return 126;
            break;
        case MCHW(O, I, I, I, I, I, I, I):
            return 127;
            break;
        case MCHW(I, O, O, O, O, O, O, O):
            return 128;
            break;
        case MCHW(I, O, O, O, O, O, O, I):
            return 129;
            break;
        case MCHW(I, O, O, O, O, O, I, O):
            return 130;
            break;
        case MCHW(I, O, O, O, O, O, I, I):
            return 131;
            break;
        case MCHW(I, O, O, O, O, I, O, O):
            return 132;
            break;
        case MCHW(I, O, O, O, O, I, O, I):
            return 133;
            break;
        case MCHW(I, O, O, O, O, I, I, O):
            return 134;
            break;
        case MCHW(I, O, O, O, O, I, I, I):
            return 135;
            break;
        case MCHW(I, O, O, O, I, O, O, O):
            return 136;
            break;
        case MCHW(I, O, O, O, I, O, O, I):
            return 137;
            break;
        case MCHW(I, O, O, O, I, O, I, O):
            return 138;
            break;
        case MCHW(I, O, O, O, I, O, I, I):
            return 139;
            break;
        case MCHW(I, O, O, O, I, I, O, O):
            return 140;
            break;
        case MCHW(I, O, O, O, I, I, O, I):
            return 141;
            break;
        case MCHW(I, O, O, O, I, I, I, O):
            return 142;
            break;
        case MCHW(I, O, O, O, I, I, I, I):
            return 143;
            break;
        case MCHW(I, O, O, I, O, O, O, O):
            return 144;
            break;
        case MCHW(I, O, O, I, O, O, O, I):
            return 145;
            break;
        case MCHW(I, O, O, I, O, O, I, O):
            return 146;
            break;
        case MCHW(I, O, O, I, O, O, I, I):
            return 147;
            break;
        case MCHW(I, O, O, I, O, I, O, O):
            return 148;
            break;
        case MCHW(I, O, O, I, O, I, O, I):
            return 149;
            break;
        case MCHW(I, O, O, I, O, I, I, O):
            return 150;
            break;
        case MCHW(I, O, O, I, O, I, I, I):
            return 151;
            break;
        case MCHW(I, O, O, I, I, O, O, O):
            return 152;
            break;
        case MCHW(I, O, O, I, I, O, O, I):
            return 153;
            break;
        case MCHW(I, O, O, I, I, O, I, O):
            return 154;
            break;
        case MCHW(I, O, O, I, I, O, I, I):
            return 155;
            break;
        case MCHW(I, O, O, I, I, I, O, O):
            return 156;
            break;
        case MCHW(I, O, O, I, I, I, O, I):
            return 157;
            break;
        case MCHW(I, O, O, I, I, I, I, O):
            return 158;
            break;
        case MCHW(I, O, O, I, I, I, I, I):
            return 159;
            break;
        case MCHW(I, O, I, O, O, O, O, O):
            return 160;
            break;
        case MCHW(I, O, I, O, O, O, O, I):
            return 161;
            break;
        case MCHW(I, O, I, O, O, O, I, O):
            return 162;
            break;
        case MCHW(I, O, I, O, O, O, I, I):
            return 163;
            break;
        case MCHW(I, O, I, O, O, I, O, O):
            return 164;
            break;
        case MCHW(I, O, I, O, O, I, O, I):
            return 165;
            break;
        case MCHW(I, O, I, O, O, I, I, O):
            return 166;
            break;
        case MCHW(I, O, I, O, O, I, I, I):
            return 167;
            break;
        case MCHW(I, O, I, O, I, O, O, O):
            return 168;
            break;
        case MCHW(I, O, I, O, I, O, O, I):
            return 169;
            break;
        case MCHW(I, O, I, O, I, O, I, O):
            return 170;
            break;
        case MCHW(I, O, I, O, I, O, I, I):
            return 171;
            break;
        case MCHW(I, O, I, O, I, I, O, O):
            return 172;
            break;
        case MCHW(I, O, I, O, I, I, O, I):
            return 173;
            break;
        case MCHW(I, O, I, O, I, I, I, O):
            return 174;
            break;
        case MCHW(I, O, I, O, I, I, I, I):
            return 175;
            break;
        case MCHW(I, O, I, I, O, O, O, O):
            return 176;
            break;
        case MCHW(I, O, I, I, O, O, O, I):
            return 177;
            break;
        case MCHW(I, O, I, I, O, O, I, O):
            return 178;
            break;
        case MCHW(I, O, I, I, O, O, I, I):
            return 179;
            break;
        case MCHW(I, O, I, I, O, I, O, O):
            return 180;
            break;
        case MCHW(I, O, I, I, O, I, O, I):
            return 181;
            break;
        case MCHW(I, O, I, I, O, I, I, O):
            return 182;
            break;
        case MCHW(I, O, I, I, O, I, I, I):
            return 183;
            break;
        case MCHW(I, O, I, I, I, O, O, O):
            return 184;
            break;
        case MCHW(I, O, I, I, I, O, O, I):
            return 185;
            break;
        case MCHW(I, O, I, I, I, O, I, O):
            return 186;
            break;
        case MCHW(I, O, I, I, I, O, I, I):
            return 187;
            break;
        case MCHW(I, O, I, I, I, I, O, O):
            return 188;
            break;
        case MCHW(I, O, I, I, I, I, O, I):
            return 189;
            break;
        case MCHW(I, O, I, I, I, I, I, O):
            return 190;
            break;
        case MCHW(I, O, I, I, I, I, I, I):
            return 191;
            break;
        case MCHW(I, I, O, O, O, O, O, O):
            return 192;
            break;
        case MCHW(I, I, O, O, O, O, O, I):
            return 193;
            break;
        case MCHW(I, I, O, O, O, O, I, O):
            return 194;
            break;
        case MCHW(I, I, O, O, O, O, I, I):
            return 195;
            break;
        case MCHW(I, I, O, O, O, I, O, O):
            return 196;
            break;
        case MCHW(I, I, O, O, O, I, O, I):
            return 197;
            break;
        case MCHW(I, I, O, O, O, I, I, O):
            return 198;
            break;
        case MCHW(I, I, O, O, O, I, I, I):
            return 199;
            break;
        case MCHW(I, I, O, O, I, O, O, O):
            return 200;
            break;
        case MCHW(I, I, O, O, I, O, O, I):
            return 201;
            break;
        case MCHW(I, I, O, O, I, O, I, O):
            return 202;
            break;
        case MCHW(I, I, O, O, I, O, I, I):
            return 203;
            break;
        case MCHW(I, I, O, O, I, I, O, O):
            return 204;
            break;
        case MCHW(I, I, O, O, I, I, O, I):
            return 205;
            break;
        case MCHW(I, I, O, O, I, I, I, O):
            return 206;
            break;
        case MCHW(I, I, O, O, I, I, I, I):
            return 207;
            break;
        case MCHW(I, I, O, I, O, O, O, O):
            return 208;
            break;
        case MCHW(I, I, O, I, O, O, O, I):
            return 209;
            break;
        case MCHW(I, I, O, I, O, O, I, O):
            return 210;
            break;
        case MCHW(I, I, O, I, O, O, I, I):
            return 211;
            break;
        case MCHW(I, I, O, I, O, I, O, O):
            return 212;
            break;
        case MCHW(I, I, O, I, O, I, O, I):
            return 213;
            break;
        case MCHW(I, I, O, I, O, I, I, O):
            return 214;
            break;
        case MCHW(I, I, O, I, O, I, I, I):
            return 215;
            break;
        case MCHW(I, I, O, I, I, O, O, O):
            return 216;
            break;
        case MCHW(I, I, O, I, I, O, O, I):
            return 217;
            break;
        case MCHW(I, I, O, I, I, O, I, O):
            return 218;
            break;
        case MCHW(I, I, O, I, I, O, I, I):
            return 219;
            break;
        case MCHW(I, I, O, I, I, I, O, O):
            return 220;
            break;
        case MCHW(I, I, O, I, I, I, O, I):
            return 221;
            break;
        case MCHW(I, I, O, I, I, I, I, O):
            return 222;
            break;
        case MCHW(I, I, O, I, I, I, I, I):
            return 223;
            break;
        case MCHW(I, I, I, O, O, O, O, O):
            return 224;
            break;
        case MCHW(I, I, I, O, O, O, O, I):
            return 225;
            break;
        case MCHW(I, I, I, O, O, O, I, O):
            return 226;
            break;
        case MCHW(I, I, I, O, O, O, I, I):
            return 227;
            break;
        case MCHW(I, I, I, O, O, I, O, O):
            return 228;
            break;
        case MCHW(I, I, I, O, O, I, O, I):
            return 229;
            break;
        case MCHW(I, I, I, O, O, I, I, O):
            return 230;
            break;
        case MCHW(I, I, I, O, O, I, I, I):
            return 231;
            break;
        case MCHW(I, I, I, O, I, O, O, O):
            return 232;
            break;
        case MCHW(I, I, I, O, I, O, O, I):
            return 233;
            break;
        case MCHW(I, I, I, O, I, O, I, O):
            return 234;
            break;
        case MCHW(I, I, I, O, I, O, I, I):
            return 235;
            break;
        case MCHW(I, I, I, O, I, I, O, O):
            return 236;
            break;
        case MCHW(I, I, I, O, I, I, O, I):
            return 237;
            break;
        case MCHW(I, I, I, O, I, I, I, O):
            return 238;
            break;
        case MCHW(I, I, I, O, I, I, I, I):
            return 239;
            break;
        case MCHW(I, I, I, I, O, O, O, O):
            return 240;
            break;
        case MCHW(I, I, I, I, O, O, O, I):
            return 241;
            break;
        case MCHW(I, I, I, I, O, O, I, O):
            return 242;
            break;
        case MCHW(I, I, I, I, O, O, I, I):
            return 243;
            break;
        case MCHW(I, I, I, I, O, I, O, O):
            return 244;
            break;
        case MCHW(I, I, I, I, O, I, O, I):
            return 245;
            break;
        case MCHW(I, I, I, I, O, I, I, O):
            return 246;
            break;
        case MCHW(I, I, I, I, O, I, I, I):
            return 247;
            break;
        case MCHW(I, I, I, I, I, O, O, O):
            return 248;
            break;
        case MCHW(I, I, I, I, I, O, O, I):
            return 249;
            break;
        case MCHW(I, I, I, I, I, O, I, O):
            return 250;
            break;
        case MCHW(I, I, I, I, I, O, I, I):
            return 251;
            break;
        case MCHW(I, I, I, I, I, I, O, O):
            return 252;
            break;
        case MCHW(I, I, I, I, I, I, O, I):
            return 253;
            break;
        case MCHW(I, I, I, I, I, I, I, O):
            return 254;
            break;
        case MCHW(I, I, I, I, I, I, I, I):
            return 255;
            break;
    };
   
    assert("Non-valid Manchester-code encountered" == NULL);
    return 0; /*Silence compiler*/
}
