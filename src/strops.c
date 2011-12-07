#include "strops.h"

char *str_Cat(char *a,char *b)
{
	 char *tmp = (char*)mem_malloc(strlen(a)+strlen(b)+1);
	 memset(tmp,0,strlen(a)+strlen(b)+1);
	 memcpy(tmp,a,strlen(a));
	 memcpy(tmp+strlen(a),b,strlen(b));
     return(tmp);

}