
#include "kermit.h"
#include "crc_ccitt.h"
#include <stdio.h>

/**
 * @brief Obtiene el Checksum del paquete.
 * @details 
 *      Hace el cálculo del checksum del paquete y lo almacena
 *      dentro del campo 'check' del mismo.
 * 
 * @param pack paquete a calcular el cheksum
 * @return     Devuelve el valor del checksum
 */
uint16_t kermit_get_package_checksum(kermit_package_t* p)
{
    int bufferSize = p->len - KERMIT_PACKAGE_CONTROL_SIZE;
    uint16_t crc = VALOR_INICIAL;
    uint8_t i;

    crc = crc_process_byte(crc,p->len);
    crc = crc_process_byte(crc,p->seq);
    crc = crc_process_byte(crc,p->type);

    uint8_t* data = p->data;

    while(bufferSize--) 
        crc = crc_process_byte(crc, *data++);

    crc ^= VALOR_XOR;
  
    return crc;

}

uint8_t kermit_check_package_checksum(kermit_package_t* p)
{
   uint16_t crc = kermit_get_package_checksum(p);
   return !(crc & p->check);
}

int kermit_set_new_package(kermit_package_t* p, 
                           uint8_t packageNumber, 
                           kermit_package_type_t package_type, 
                           uint8_t* dataBuffer, 
                           uint8_t dataSize)
{

    return 0;
}

void kermit_print_package(kermit_package_t p)
{
    int dataSize = p.len - KERMIT_PACKAGE_CONTROL_SIZE;
    uint8_t blank_bytes= 8 - ((dataSize % 8) ? dataSize % 8:8);
    uint8_t i;
    printf("*********************************************************************\n");
    printf("*\t\t\t    Package Info\t\t\t    *\n");
    printf("*-------------------------------------------------------------------*\n");
    printf("*  Package MARK       = 0x%X\t\t\t\t\t    *\n", p.mark);
    printf("*  Package LEN        = %d bytes\t\t\t\t    *\n", p.len);
    printf("*  Package SEQUENCE   = %d\t\t\t\t\t    *\n", p.seq);
    printf("*  Package TYPE       = %c\t\t\t\t\t    *\n", p.type);
    printf("*  Package DATA       = {\t\t\t\t\t    *\n*\t\t\t  ");
    
    for(i = 0; i < dataSize; ++i) {
        printf("0x%X",p.data[i]);
        if (i<dataSize-1)
        {
            printf(",");
            if(!((i+1)%8) )
                printf("  *\n*\t\t\t  ");
        }

    }
    for (int i = 0; i < blank_bytes; ++i)
    {
        printf("    ");
    }
    printf("\t    *\n*\t\t        }\t\t\t\t\t    *\n");
    
    printf("*  Package CHECK      = 0x%.4X\t\t\t\t\t    *\n", p.check);
    printf("*********************************************************************\n");
}   
