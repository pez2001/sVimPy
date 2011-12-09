#include "numops.h"

long long_pow(long base,long exp)
{
long r =base;
for(long i=0;i<exp-1;i++)
{
 r = r * base;
}
return(r);
}

short num_short(unsigned char a,unsigned char b)
{
//printf("num_short:%d\n",(a<<8 + b));
return((b<<8) + a);
}


