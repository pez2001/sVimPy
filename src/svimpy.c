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

#include "svimpy.h"

#ifdef USE_DEBUGGING
int debug_level = 0;
#endif

void ExecutePYC(char *filename)
{
	//printf("executing object:%s\n", filename);
	vm *vm = vm_Init(NULL);
	gc_Init(vm);
	streams_Init();
	AddInternalFunctions(vm);
	//fmod_Init(vm);

	long pyc_magic = MAGIC;
	stream *f = stream_CreateFromFile(filename,"rb");
	if (!stream_Open(f))
		return;
	long magic = ReadLong(f);
	if (magic != pyc_magic)
		return;
	ReadLong(f);//skip time
	object *obj = ReadObject(f);
	vm_AddGlobal(vm, (code_object*)obj);
	object *ret = NULL;
	ret = vm_RunObject(vm, obj, NULL);	// ,obj
	if (ret != NULL)
	{
		#ifdef USE_DEBUGGING
		if((debug_level & DEBUG_DUMP_OBJECT) > 0)
			DumpObject(ret, 0);
		#endif
		gc_DecRefCount(ret);
	}
	vm_RemoveGlobal(vm,(code_object*)obj);
	gc_DecRefCount(obj);
	stream_Free(f);
	gc_Clear();
	vm_Close(vm);
	//fmod_Close();
	streams_Close();
	gc_Close();
}

void ExecuteRPYC(char *filename)
{
	//printf("executing object:%s\n", filename);
	vm *vm = vm_Init(NULL);
	gc_Init(vm);
	streams_Init();
	AddInternalFunctions(vm);
	//fmod_Init(vm);
	stream *f = stream_CreateFromFile(filename,"rb");
	if (!stream_Open(f))
		return;
	object *obj = ReadObject(f);
	vm_AddGlobal(vm, (code_object*)obj);
	object *ret = NULL;
	ret = vm_RunObject(vm, obj, NULL);	// ,obj
	if (ret != NULL)
	{
		#ifdef USE_DEBUGGING
		if((debug_level & DEBUG_DUMP_OBJECT) > 0)
			DumpObject(ret, 0);
		#endif
		gc_DecRefCount(ret);
	}
	vm_RemoveGlobal(vm,(code_object*)obj);
	gc_DecRefCount(obj);
	stream_Free(f);
	gc_Clear();
	vm_Close(vm);
	//fmod_Close();
	streams_Close();
	gc_Close();
}

void ExecuteRPYC_PLUS(char *filename)
{
	//printf("executing object:%s\n", filename);
	vm *vm = vm_Init(NULL);
	gc_Init(vm);
	streams_Init();
	AddInternalFunctions(vm);
	//fmod_Init(vm);

	stream *f = stream_CreateFromFile(filename,"rb");
	if (!stream_Open(f))
		return;
	object *obj = ReadObjectPlus(f);
	vm_AddGlobal(vm, (code_object*)obj);
	object *ret = NULL;
	ret = vm_RunObject(vm, obj, NULL);	// ,obj
	if (ret != NULL)
	{
		#ifdef USE_DEBUGGING
		if((debug_level & DEBUG_DUMP_OBJECT) > 0)
			DumpObject(ret, 0);
		#endif
		gc_DecRefCount(ret);
	}
	vm_RemoveGlobal(vm,(code_object*)obj);
	gc_DecRefCount(obj);
	stream_Free(f);
	gc_Clear();
	vm_Close(vm);
	//fmod_Close();
	streams_Close();
	gc_Close();
}

int main(int argc, char** argv)
{
	int c = 0;
	int print_object = 0;
	int format_pyc = 1;
	int format_rpyc = 0;
	int format_rpyc_plus = 0;
	//int print_object = 0;
	char *filename = NULL;
	#ifdef USE_DEBUGGING
	while ((c = getopt (argc, argv, "if:ldohv")) != -1)
	#else
	while ((c = getopt (argc, argv, "if:ohv")) != -1)
	#endif
	{
		switch (c)
		{
			case 'f':
				if(strcasecmp(optarg,"RPYC")==0)
				{
					format_pyc = 0;
					format_rpyc = 1;
					format_rpyc_plus = 0;
				} else
				if(strcasecmp(optarg,"RPYC+")==0)
				{
					format_pyc = 0;
					format_rpyc = 0;
					format_rpyc_plus = 1;
				}
				//fprintf (stderr,"\n");
				//id=atoi(optarg);
				break;
			#ifdef USE_DEBUGGING
			case 'd':
				{
					//#ifdef USE_DEBUGGING
					//debug_level |= DEBUG_DUMP_UNSUPPORTED;
					//#endif
					DumpUnsupportedOpCodes();
					return(0);
				}
			#endif
			case 'o':
				print_object = 1;
				break;
			case 'h':
				printf(svimpy_helpmsg);
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
	filename = argv[optind];
	if(filename != NULL && strlen(filename)>0 && print_object)
	{
		#ifdef USE_DEBUGGING	
		debug_level |= DEBUG_FULL_DUMP;
		#endif
		gc_Init(NULL);
		streams_Init();
		long pyc_magic = MAGIC;
		stream *f = stream_CreateFromFile(filename,"rb");
		if (!stream_Open(f))
			return(1);
		//printf("file opened\n");
		long magic = ReadLong(f);

		if (magic != pyc_magic)
			return(1);
		ReadLong(f);//skip time
		object *obj = ReadObject(f);
		#ifdef USE_DEBUGGING	
		DumpObject(obj, 0);
		#else
		PrintObject(obj);
		#endif
		gc_DecRefCount(obj);
		gc_Clear();
		stream_Free(f);
		streams_Close();
		gc_Close();
		return (0);
	}else
	if(filename != NULL && strlen(filename)>0 && format_pyc) //execute file as default option
		ExecutePYC(filename);
	else if(filename != NULL && strlen(filename)>0 && format_rpyc) //use rpyc format
		ExecuteRPYC(filename);
	else if(filename != NULL && strlen(filename)>0 && format_rpyc_plus) //use rpyc+ format
		ExecuteRPYC_PLUS(filename);
	else 
		printf(svimpy_helpmsg);

}
	
	