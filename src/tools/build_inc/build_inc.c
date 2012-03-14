#include "build_inc.h"


int main(int argc, char *argv[])
{
	printf("Makefile filename:%s\n",argv[1]);
	printf("Makefile variable to increment:%s\n",argv[2]);
	FILE *f = fopen(argv[1],"r");
	FILE *o = fopen("Makefile.new","w");
	while(!feof(f))
	{
		char line[300000];
		fgets(&line,300000,f);
		//printf("%s\n",&line);
		if(strstr(&line,argv[2]) == &line)
		{
			int count = 0;
			//printf("line:%s\n",&line);
			//sscanf(&line,"%*[] = %d",&count);
			//int pos = strpos(&line,"=");
			//count = atoi(strstr(&line,"="));
			char *tok;
			tok = strtok (&line," =");
			int i = 0;
			while (tok != NULL)
			{
				//printf("%s\n",tok);
				if(i == 1) 
					count = atoi(tok);
				tok = strtok (NULL," =");
				i++;
			}
			
			count++;
			printf("Incremented %s = %d\n",&line,count);
			//int count = 
			//count++;
			fprintf(o,"%s = %d\n",argv[2],count);
		}
		else
			fprintf(o,"%s",&line);
	}
	fclose(f);
	fclose(o);
	remove(argv[1]);
	rename("Makefile.new",argv[1]);
	return (0);
}
