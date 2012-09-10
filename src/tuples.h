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


#ifndef TUPLES_H
#define TUPLES_H
#include "object.h"
#include "types.h"
#include "features.h"
#include "memory.h"


#ifdef __cplusplus
extern "C"  {
#endif



BOOL tuple_IsTuple(TUPLE_ID tuple);

NUM tuple_GetLen(TUPLE_ID tuple);

void tuple_ClearDictValues(TUPLE_ID tuple);

void tuple_ConvertToDict(TUPLE_ID tuple);

void tuple_Clear(TUPLE_ID tuple);

void tuple_ResetIteration(TUPLE_ID tuple);

OBJECT_ID tuple_GetNextItem(TUPLE_ID tuple);

OBJECT_ID tuple_GetItem(TUPLE_ID tuple, INDEX index);

OBJECT_ID tuple_GetDictItem(TUPLE_ID tuple,OBJECT_ID key);

OBJECT_ID tuple_GetDictItemByIndex(TUPLE_ID tuple,INDEX index);

OBJECT_ID tuple_GetDictItemByName(TUPLE_ID tuple,BYTES_ID name);

INDEX tuple_GetItemIndex(TUPLE_ID tuple,OBJECT_ID obj);

INDEX tuple_GetItemIndexByName(TUPLE_ID tuple, BYTES_ID name);

INDEX tuple_GetDictItemIndex(TUPLE_ID tuple,OBJECT_ID key);

void tuple_SetItem(TUPLE_ID tuple, INDEX index, OBJECT_ID obj);

void tuple_SetDictItem(TUPLE_ID tuple,OBJECT_ID key,OBJECT_ID value);

void tuple_SetDictItemByIndex(TUPLE_ID tuple,INDEX index,OBJECT_ID value);

void tuple_AppendItem(TUPLE_ID tuple,OBJECT_ID value);

void tuple_AppendDictItem(TUPLE_ID tuple,OBJECT_ID key,OBJECT_ID value);

void tuple_InsertItem(TUPLE_ID tuple,INDEX index,OBJECT_ID value);

void tuple_DeleteItem(TUPLE_ID tuple, INDEX index);

void tuple_DeleteDictItem(TUPLE_ID tuple,OBJECT_ID key);


#ifdef __cplusplus
} 
#endif



#endif
