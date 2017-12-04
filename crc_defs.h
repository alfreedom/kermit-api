#ifndef CRC_CCITT_H
#define CRC_CCITT_H

#include <stdint.h>

#define TRUE    1
#define FALSE   0


/* Parámetros para calcular CRC CCITT */
typedef uint16_t crc_t;
#define POLINOMIAL      0x1021
#define VALOR_INICIAL   0x1D0F
#define VALOR_XOR       0x0000
#define REVERSED        FALSE
#define MSB_MASK        0x8000




crc_t crcProcessBuffer(uint8_t* buffer, long size);

crc_t crcProcessBuffer(uint8_t* buffer, long size){
    
    crc_t crc = VALOR_INICIAL;
    uint8_t i;

    while(size--)
    {
        crc ^= ((crc_t) *buffer++) << ((8*sizeof(crc_t))-8);

        for (i = 0; i < 8; ++i)
        {
            if(crc & MSB_MASK)
                crc = (crc << 1) ^ POLINOMIAL;
            else
                crc <<= 1;
        }
        
    }
   
    crc ^= VALOR_XOR;

    return crc;

}

#endif // CRC_CCITT_H
