#ifndef CRC_CCITT_H
#define CRC_CCITT_H

#include <stdint.h>

/* Parámetros para calcular CRC CCITT */

#define POLINOMIAL      0x1021
#define VALOR_INICIAL   0x1D0F
#define VALOR_XOR       0x0000
#define REVERSED        FALSE
#define MSB_MASK        0x8000

uint16_t crc_process_byte(uint16_t crc,uint8_t b);

uint16_t crc_process_byte(uint16_t crc,uint8_t b)
{

    uint8_t i;

    // Se hace una XOR del dato con el valor del CRC
    crc ^= ((uint16_t)b) << ((8*sizeof(uint16_t))-8);

    for (i = 0; i < 8; ++i)
    {
        if(crc & MSB_MASK)
            crc = (crc << 1) ^ POLINOMIAL;
        else
            crc <<= 1;
    }

    return crc;
}


#endif // CRC_CCITT_H
