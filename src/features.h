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


#ifndef FEATURES_H
#define FEATURES_H



//#define USE_CALL_FUNCTION_VAR
//#define USE_CALL_FUNCTION_VAR_KW
//#define USE_CALL_FUNCTION_KEYWORDS

//#define USE_CLASSES
//#define USE_EXCEPTIONS
//#define USE_LOCKING
//#define USE_INTERACTIVE_CONSOLE

//#define USE_GENERATORS

//#define USE_EXTENDED_INTERFACE

//#define USE_OUTPUT

//#define USE_INPUT

//#define USE_SERIAL_STREAM

#define USE_FILE_STREAM

#define USE_INTERNAL_FUNCTIONS

#define USE_INTERNAL_CLASSES

#define USE_MEMORY_DEBUGGING

//#define USE_MEMORY_LOCK_DEBUGGING


#define USE_MEMORY_MANAGER_PASS
//#define USE_MEMORY_MANAGER_DEBUG
//#define USE_MEMORY_MANAGER_CACHE
//#define USE_MEMORY_MANAGER_CACHE_DEBUG
//#define USE_MEMORY_MANAGER_POOL
//#define USE_MEMORY_MANAGER_POOL_DEBUG

#ifdef USE_MEMORY_MANAGER_POOL_DEBUG
#define MEMORY_POOL_SIZE 2048
#endif
#ifdef USE_MEMORY_MANAGER_POOL
#define MEMORY_POOL_SIZE 2048
#endif



#ifdef USE_MEMORY_MANAGER_PASS
#undef USE_MEMORY_DEBUGGING
#endif

#ifdef USE_MEMORY_MANAGER_CACHE
#undef USE_MEMORY_DEBUGGING
#endif


//#define USE_ARDUINO_FUNCTIONS

//#define USE_FMOD_FUNCTIONS

#ifdef USE_ARDUINO_FUNCTIONS
#undef USE_DEBUGGING
#undef USE_MEMORY_DEBUGGING
#undef USE_FILE_STREAM
#undef USE_INTERNAL_CLASSES
//#define USE_ARDUINO_DEBUGGING
//#define USE_ARDUINO_OPCODE_DEBUGGING
#endif


#endif