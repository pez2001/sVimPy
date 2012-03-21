/* 
 * sVimPy - small Virtual interpreting machine for Python
 * (c) 2012 by Tim Theede aka Pez2001 <pez2001@voyagerproject.de> / vp
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * THIS SOFTWARE IS SUPPLIED AS IT IS WITHOUT ANY WARRANTY!
 *
 */

//very basic code to automatically increment the build number in a Makefile
 
#include "build_inc.h"


int main(int argc, char *argv[])
{
	if(argv[1] == NULL || argv[2] == NULL)
	{
		printf("No Makefile / Variable given as parameter(s)\n");
		return(1);
	}

	printf("Makefile filename:%s\n",argv[1]);
	printf("Makefile variable to increment:%s\n",argv[2]);
	FILE *f = fopen(argv[1],"r");
	FILE *o = fopen("Makefile.new","w");
	while(!feof(f))
	{
		char line[300000];
		memset((char*)&line,0,300000);
		if(fgets((char*)&line,300000,f)==NULL)
			break;
		//printf("%s\n",&line);
		if(strstr((char*)&line,argv[2]) == (char*)&line)
		{
			int count = 0;
			//printf("line:%s\n",&line);
			//sscanf(&line,"%*[] = %d",&count);
			//int pos = strpos(&line,"=");
			//count = atoi(strstr(&line,"="));
			char *tok;
			tok = strtok ((char*)&line," =\n");
			int i = 0;
			while (tok != NULL)
			{
				//printf("%s\n",tok);
				if(i == 1) 
					count = atoi(tok);
				tok = strtok (NULL," =\n");
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
