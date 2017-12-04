/**
 *     kermit.h
 *
 *         Versión:  1.0
 *
 *           Autor:  Alfredo Orozco de la Paz
 *         Archivo:  kermit.h
 *           Fecha:  Junio 5 del 2015
 *
 *  ############################# Última Modficación #############################
 *      
 *  ##############################################################################
 ***************************************************************************************
 *  Descripción:   
 *
 *   Librería para el manejo de transferencia de archivos mediante el protocolo
 *   kermit.
 *   
 *   Esta librería sirve para manipular paquetes que seran enviados mediante
 *   el protocolo kermit.
 *   
 *   
 *   El formato del paquete de datos para el protocolo se forma de la siguiente
 *   manera:
 *      
 *              -----------------------------------------------------
 *    Campos    | MARK | LEN | SEQ | TYPE | DATA* | CHECKL | CHECKH |
 *              -----------------------------------------------------
 *    Bytes #   |   1  |  1  |  1  |   1  |   N   |    1   |    1   |
 *              -----------------------------------------------------
 *              
 *              Bytes en total: 6 + N.
 *              
 *              El tamaño total del paquete no debe ser mayor a 96, por
 *              lo que N seria:  
 *                                   0 < N < 90
 *                                   
 *      MARK:   Caracter de sicronizacion. Normalmente es SOH (0x01)
 *              pero puede ser cualquiera siempre y cuando se especifique tanto
 *              en el receptor como en el emisor.
 *             
 *      LEN:    Tamaño del paquete. Especifica el numero de bytes que contiene
 *              el paquete a partir de este campo, es decir, no se cuentan los
 *              bytes MARK ni el mismo LEN.
 *                          
 *                         LEN = SEQ + TYPE + DATA + CHECK-L + CHECK-H
 *                         
 *      SEQ:    Numero de secuencia del paquete. Indica el numero del paquete
 *              a enviar, es un numero de 0 a 63. El numero de paquete se 
 *              reinicia a 0 si es mayor que 63 (modulo 64).
 *              
 *      TYPE:   Tipo de paquete. Define el tipo de paquete a enviar, es un
 *              simple caracter ASCII. Los posibles tipos de paquetes son:
 *                  
 *                  D   Paquete de datos
 *                  Y   Paquete de Acknowledge
 *                  N   Paquete de NO Acknowledge
 *                  S   Paquete de Inicio de Transferencia
 *                  B   Paquete de Fin de Transferencia (BREAK o EOT)
 *                  F   Paquete de Cabecera de Archivo (para el nombre del archivo)
 *                  Z   Paquete de Fin de Archivo (EOF)
 *                  E   Paquete de Error.
 *                  
 *      DATA:   Datos a enviar. Son los datos que se enviarán, se aceptan como
 *              máximo 90 bytes de datos. Estos pueden ser cualquier dato de 
 *              8 bits.
 *              
 *      CHECKL/CHECKH:  Es el campo de verificación. Se envia un checksum de 
 *                      16 todos los campos anteriores a este. Sirve para
 *                      verificar que el paquete se transmitio correctamente.
 *                      Se divide en 2 bytes, CHECK-L y CHECK-H, que son el
 *                      byte menos significativo y más significativo respectivamente.
 * 
 */

#ifndef _KERMIT_H_
#define _KERMIT_H_

#include <stdint.h>


#define KERMIT_MAX_DATA_LEN          90      /* MAX_DATA_LEN = Numero máximo de datos (bytes) a enviar en el paquete */
#define KERMIT_PACKAGE_CONTROL_SIZE  4       /* Tamaño del packete sin datos: SEQ + TYPE + CHECKL + CHECKH*/

/* CARACTERES DE CONTROL */
typedef enum{

    KERMIT_MARK_START       = 0x01,            /* Caracter SOH de sincronización, puede ser cualquier otro */
    KERMIT_MARK_SPACE       = 0x20,            /* Caracter SPACE */
    KERMIT_MARK_RETURN      = 0x0D,            /* Caracter Carriage Return (CR) - Retorno de Carro */
    KERMIT_MARK_NEWLINE     = 0x0A,            /* Caracter Line Fee (LF) - Nueva Linea */
    KERMIT_MARK_DELETE      = 0x7F            /* Caracter DEL */
}kermit_mark_t;

/* TIPOS DE PAQUETES */
 typedef enum{

    KERMIT_PACKAGE_TYPE_ACK         = 'Y',         /* Acknowledge - Paquete de reconocimiento */
    KERMIT_PACKAGE_TYPE_NACK        = 'N',         /* No Aclnowledge - Packete de No reconocimiento */
    KERMIT_PACKAGE_TYPE_SEND        = 'S',         /* Send-Initiate - Packete de inicializacion */
    KERMIT_PACKAGE_TYPE_FILE        = 'F',         /* File Header - Packete de cabecera de archivo */
    KERMIT_PACKAGE_TYPE_DATA        = 'D',         /* Data - Paquete de datos */
    KERMIT_PACKAGE_TYPE_BREAK       = 'B',         /* Break Transmission - Paquete de fin de transmision*/
    KERMIT_PACKAGE_TYPE_EOF         = 'Z',         /* End-Of-File - Paquete de fin de archivo */
    KERMIT_PACKAGE_TYPE_ERROR       = 'E'          /* Error - Paquete de Error */
 }kermit_package_type_t;



#define TOCHAR(x)   ( (uint8_t) ((x)+32) )
#define UCHAR(x)    ( (uint8_t) ((x)-32) )
#define CTL(x)      ( (x) ^ 64 )

typedef uint8_t byte;

/**
 * @brief Estructura de datos del paquete.
 * @details
*/
typedef struct{
    kermit_mark_t           mark;
    uint8_t                 len;
    uint8_t                 seq;
    kermit_package_type_t   type;
    uint8_t                 data[KERMIT_MAX_DATA_LEN];
    uint16_t                check;    
}kermit_package_t;


uint16_t kermit_get_package_checksum(kermit_package_t* p);
uint8_t kermit_check_package_checksum(kermit_package_t* p);

int kermit_set_new_package(kermit_package_t* p, uint8_t packageNumber, kermit_package_type_t package_type, uint8_t* dataBuffer, uint8_t dataSize);

void kermit_print_package(kermit_package_t p);

#endif // _KERMIT_H_
