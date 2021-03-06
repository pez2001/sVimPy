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

#include "compress_pyc.h"

#ifdef USE_DEBUGGING
int debug_level = 0;
#endif

//TODO add possibility to use an even more reduced format for code objects (no filename,lnotab,firstlineno,stack_size) - perhaps decrease int sizes as well ->rpyc+ format

int main(int argc, char *argv[])
{	
	int c = 0;
	int format_rpyc = 1;
	int format_rpyc_plus = 0;
	char *input_filename = NULL;
	char *output_filename = NULL;
	
	while ((c = getopt (argc, argv, "f:hv")) != -1)
	{
		switch (c)
		{
			case 'f':
				if(strcasecmp(optarg,"RPYC+")==0)
				{
					format_rpyc = 0;
					format_rpyc_plus = 1;
				}
				break;
			case 'h':
				printf(compress_pyc_helpmsg);
				return(0);
			case 'v':
				printf("Version : %d.%d-%d\n",MAJOR_VERSION,MINOR_VERSION,BUILD+1);
				return(0);
			case '?':
				if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint(optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
				return(1);
			default:
				abort();

		}
	}
	if(optind<argc)
		input_filename = argv[optind];
	if((optind+1)<argc)
		output_filename = argv[optind+1];
	if(input_filename != NULL && strlen(input_filename)>0 && output_filename != NULL && strlen(output_filename)>0)
	{
		printf("input pyc filename:%s\n",input_filename);
		printf("output filename:%s\n",output_filename);
		mem_Init();
		obj_Init();
		streams_Init();
		long pyc_magic = MAGIC;
		STREAM_ID f = stream_CreateFromFile(mem_create_string(input_filename),mem_create_string("rb"));
		if (!stream_Open(f))
			return(1);
		STREAM_ID o = stream_CreateFromFile(mem_create_string(output_filename),mem_create_string("wb+"));
		if (!stream_Open(o))
			return(1);
		printf("files opened\n");
		long magic = stream_ReadLong(f);

		if(magic != pyc_magic)	
			return(1);
		stream_ReadLong(f);//skip time
		OBJECT_ID obj = stream_ReadObject(f);
		
		//obj_SetAttribute(obj, //TODO remove doc strings from objects
		if(format_rpyc)
		{
			stream_WriteObject(obj,o);
			printf("written object to stream in rpyc format\n");	
		}
		else if(format_rpyc_plus)
		{
			stream_WriteObjectPlus(obj,o);
			printf("written object to stream in rpyc+ format\n");
		}
		stream_Free(f);
		stream_Free(o);
		streams_Close();
		obj_Close();
	}
	else
		printf(compress_pyc_helpmsg);

	return(0);
}
