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


#ifndef SVIMPY_H
#define SVIMPY_H

#include "features.h"
#include "vm.h"

#include "libraries/fmod_wrapper.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <sys/types.h>
#include <getopt.h>

int strcasecmp(const char *s1, const char *s2);

#ifdef USE_DEBUGGING
const char *svimpy_helpmsg = "Usage: svimpy [OPTION]... [INPUT FILE]\n\
Execute Compiled Python Code Files.\n\
\n\
  -i            		interactive mode\n\
  -f <format>		use format <format> for input file\n\
				[pyc,rpyc,rpyc+,pos,pos+]\n\
  -o					print object contents only\n\
  -l                    print list of supported internal functions\n\
  -d					print list of unsupported opcodes\n\
  -h                    display this help and exit\n\
  -v                    output version information and exit\n\
\n\
\n\
Report bugs to <pez2001@voyagerproject.de>.\n";
#else
const char *svimpy_helpmsg = "Usage: svimpy [OPTION]... [INPUT FILE]\n\
Execute Compiled Python Code Files.\n\
\n\
  -i            		interactive mode\n\
  -f <format>		use format <format> for input file\n\
				[pyc,rpyc,rpyc+,pos,pos+]\n\
  -o					print object contents only\n\
  -h                    display this help and exit\n\
  -v                    output version information and exit\n\
\n\
\n\
Report bugs to <pez2001@voyagerproject.de>.\n";
#endif

#endif
