#include <stdio.h>
#include "kermit.h"

int main(int argc, char const *argv[])
{
 
    kermit_package_t p;
    int dataSize =30;
    
    for(int i=0; i< dataSize;i++)
        p.data[i]='1'+ i;       

    p.mark = KERMIT_MARK_START;
    p.len=4+dataSize;
    p.seq = 0;
    p.type = KERMIT_PACKAGE_TYPE_SEND;
    p.check = ~ kermit_get_package_checksum(&p);
    
    kermit_print_package(p);
    printf("CRC result = %s\n",kermit_check_package_checksum(&p) ? "OK":"ERROR");
    return 0;
}