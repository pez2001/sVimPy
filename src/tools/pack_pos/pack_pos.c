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

#include "pack_pos.h"

//just a kind of cat >> ;;
//no header just a stream of raw python objects

int main(int argc, char *argv[])
{
	if(argv[1] == NULL)
	{
		printf("No Svimpy Manifest file given as parameter\n");
		return(1);
	}
	printf("Svimpy Manifest filename:%s\n",argv[1]);
	FILE *f = fopen((char*)argv[1],"r");
	FILE *o = NULL;
	//get directory name if existing in Manifest filename
	char *filename = strrchr((char*)argv[1],'/');
	char path[512];
	memset((char*)&path,0,512);
	if(filename != NULL)
	{	
		filename+=1;//skip the /
		printf("manifest filename:[%s]\n",filename);
		strncpy((char*)&path,argv[1],strlen(argv[1])-strlen(filename));
		printf("manifest path:[%s]\n",path);
	}
	else
		strcat((char*)&path,"");
	while(!feof(f))
	{
		char line[2048];
		memset((char*)&line,0,2048);
		if(fgets((char*)&line,2048,f)==NULL)
			break;
		//printf("%s\n",&line);
		if(strstr((char*)&line,"NAME") == (char*)&line)
		{
			char *tok = NULL;
			tok = strtok ((char*)&line," =\n");
			int i = 0;
			while (tok != NULL)
			{
				if(i == 1) 
				{
					char cp[512];//complete path
					memset((char*)&cp,0,512);
					strcat((char*)&cp,path);
					strcat((char*)&cp+strlen(path),tok);
					printf("trying to open output file:[%s]\n",&cp);
					
					o = fopen((char*)&cp,"ab+");
					if (o == NULL)
						perror ("error occurred");
					if (o == NULL)
						return(1);
 					if(o != NULL)
						printf("opened output file:%s\n",&cp);
					//fseek(o,0,SEEK_END);
				}
				tok = strtok (NULL," =\n");
				i++;
			}
		}
		else if(strstr((char*)&line,"ITEM") == (char*)&line && o != NULL)
		{
			char *tok;
			tok = strtok ((char*)&line," =\n");
			int i = 0;
			while (tok != NULL)
			{
				if(i == 1)
				{
					char cp[512];//complete path
					memset((char*)&cp,0,512);
					strcat((char*)&cp,path);
					strcat((char*)&cp+strlen(path),tok);
					printf("trying to open input file:[%s]\n",&cp);
					FILE *in = fopen((char*)&cp,"rb");
					if(in == NULL)
					if (in == NULL)
						perror ("error occurred");
					if (in == NULL)
						return(1);
 					if(in != NULL)
						printf("adding item:[%s]\n",&cp);
					while(!feof(in))
					{
						char buf[512];
						int r = fread((char*)&buf,1,512,in);
						if(r== -1 || r== 0)
							break;
						fwrite((char*)&buf,r,1,o);
						
					}
					fclose(in);
				}
				tok = strtok (NULL," =\n");
				i++;
			}	
		}
		else
			printf("item set before name, skipping it\n");
		
	}
	fclose(f);
	
	fclose(o);
	return (0);
}
