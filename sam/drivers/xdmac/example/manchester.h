#ifndef MANCHESTER_H
#define MANCHESTER_H
/*

 \file manchester.h

 \brief Fast manchester encode/decode

 \author Michael Ambrus

 */

#include <stdint.h>
uint16_t manchester_encode(uint8_t);
uint8_t manchester_decode(uint16_t);

#endif //MANCHESTER_H
