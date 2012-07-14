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
	VM_ID vm = vm_Init(0);
	long pyc_magic = MAGIC;
	MEM_ID filename_id = mem_create_string(filename);
	STREAM_ID f = stream_CreateFromFile(filename_id,mem_create_string("rb"));
	//mem_free(filename_id);
	if (!stream_Open(f))
		return;
	long magic = stream_ReadLong(f);
	if (magic != pyc_magic)
		return;
	stream_ReadLong(f);//skip time
	OBJECT_ID obj = stream_ReadObject(f);
	code_object *co = (code_object*)mem_lock(obj);
	OBJECT_ID global_key = obj_CreateUnicode(mem_copy(co->name));
	mem_unlock(obj,0);
	vm_AddGlobal(vm, global_key,obj);
	OBJECT_ID ret = 0;
	ret = vm_RunObject(vm, obj, 0,0);
	if (ret != 0)
	{
		#ifdef USE_DEBUGGING
		if((debug_level & DEBUG_DUMP_OBJECT) > 0)
			obj_Dump(ret, 0,1);
		#endif
		obj_DecRefCount(ret);
	}
	vm_RemoveGlobal(vm,global_key);
	obj_DecRefCount(obj);
	stream_Free(f);
	vm_Close(vm);
}

void ExecuteRPYC(char *filename)
{
	//printf("executing object:%s\n", filename);
	VM_ID vm = vm_Init(0);
	MEM_ID filename_id = mem_create_string(filename);
	STREAM_ID f = stream_CreateFromFile(filename_id,mem_create_string("rb"));
	//mem_free(filename_id);
	if (!stream_Open(f))
		return;
	OBJECT_ID obj = stream_ReadObject(f);
	code_object *co = (code_object*)mem_lock(obj);
	OBJECT_ID global_key = obj_CreateUnicode(mem_copy(co->name));
	mem_unlock(obj,0);
	vm_AddGlobal(vm, global_key,obj);
	OBJECT_ID ret = 0;
	ret = vm_RunObject(vm, obj, 0,0);	
	if (ret != 0)
	{
		#ifdef USE_DEBUGGING
		if((debug_level & DEBUG_DUMP_OBJECT) > 0)
			obj_Dump(ret, 0,1);
		#endif
		obj_DecRefCount(ret);
	}
	vm_RemoveGlobal(vm,global_key);
	obj_DecRefCount(obj);
	stream_Free(f);
	vm_Close(vm);
}

void ExecuteRPYC_PLUS(char *filename)
{
	//printf("executing object:%s\n", filename);
	VM_ID vm = vm_Init(0);
	MEM_ID filename_id = mem_create_string(filename);
	STREAM_ID f = stream_CreateFromFile(filename_id,mem_create_string("rb"));
	//mem_free(filename_id);
	if (!stream_Open(f))
		return;
	OBJECT_ID obj = stream_ReadObjectPlus(f);
	code_object *co = (code_object*)mem_lock(obj);
	OBJECT_ID global_key = obj_CreateUnicode(mem_copy(co->name));
	mem_unlock(obj,0);
	vm_AddGlobal(vm, global_key,obj);
	OBJECT_ID ret = 0;
	ret = vm_RunObject(vm, obj, 0,0);	
	if (ret != 0)
	{
		#ifdef USE_DEBUGGING
		if((debug_level & DEBUG_DUMP_OBJECT) > 0)
			obj_Dump(ret, 0,1);
		#endif
		obj_DecRefCount(ret);
	}
	vm_RemoveGlobal(vm, global_key);
	obj_DecRefCount(obj);
	stream_Free(f);
	vm_Close(vm);
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
		mem_Init();
		obj_Init();
		streams_Init();
		long pyc_magic = MAGIC;
		MEM_ID filename_id = mem_create_string(filename);
		STREAM_ID f = stream_CreateFromFile(filename_id,mem_create_string("rb"));
		//mem_free(filename_id);
		if (!stream_Open(f))
			return(1);
		//printf("file opened\n");
		long magic = stream_ReadLong(f);

		if (magic != pyc_magic)
			return(1);
		stream_ReadLong(f);//skip time
		OBJECT_ID obj = stream_ReadObject(f);
		#ifdef USE_DEBUGGING	
		obj_Dump(obj, 0,1);
		#else
		obj_Print(obj);
		#endif
		obj_DecRefCount(obj);
		obj_ClearGC();
		stream_Free(f);
		streams_Close();
		obj_Close();
		mem_Close();
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
	
	