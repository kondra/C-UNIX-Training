#define IPCONVERT(a,b,c,d,addr) ({d=(unsigned char)(addr),c=(unsigned char)((addr)>>8),b=(unsigned char)((addr)>>16),a=(unsigned char)((addr)>>24);})
#include <task.ipconvertmacro.c> 
