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

#ifndef FADE_H
#define FADE_H

#include "avr/pgmspace.h"

#define FADE_MIN_LEN 491

//static unsigned char PROGMEM fade_min[]=
static unsigned char fade_min[]=
 {
  0x63,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x02,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x73,0x2e,0x00,0x00,0x00,
  0x64,0x00,0x00,0x64,0x01,0x00,0x6c,0x00,0x00,0x5a,0x00,0x00,0x64,
  0x00,0x00,0x61,0x01,0x00,0x64,0x02,0x00,0x61,0x02,0x00,0x64,0x03,
  0x00,0x84,0x00,0x00,0x5a,0x03,0x00,0x64,0x04,0x00,0x84,0x00,0x00,
  0x5a,0x04,0x00,0x64,0x01,0x00,0x53,0x28,0x05,0x00,0x00,0x00,0x69,
  0x00,0x00,0x00,0x00,0x4e,0x69,0x05,0x00,0x00,0x00,0x63,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,
  0x00,0x43,0x00,0x00,0x00,0x73,0x11,0x00,0x00,0x00,0x74,0x00,0x00,
  0x64,0x01,0x00,0x74,0x01,0x00,0x83,0x02,0x00,0x01,0x64,0x00,0x00,
  0x53,0x28,0x02,0x00,0x00,0x00,0x4e,0x69,0x09,0x00,0x00,0x00,0x28,
  0x02,0x00,0x00,0x00,0x75,0x07,0x00,0x00,0x00,0x70,0x69,0x6e,0x4d,
  0x6f,0x64,0x65,0x75,0x06,0x00,0x00,0x00,0x4f,0x55,0x54,0x50,0x55,
  0x54,0x28,0x00,0x00,0x00,0x00,0x28,0x00,0x00,0x00,0x00,0x28,0x00,
  0x00,0x00,0x00,0x75,0x00,0x00,0x00,0x00,0x75,0x05,0x00,0x00,0x00,
  0x73,0x65,0x74,0x75,0x70,0x00,0x00,0x00,0x00,0x4e,0x63,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,
  0x00,0x43,0x00,0x00,0x00,0x73,0x47,0x00,0x00,0x00,0x74,0x00,0x00,
  0x64,0x01,0x00,0x74,0x01,0x00,0x83,0x02,0x00,0x01,0x74,0x01,0x00,
  0x74,0x02,0x00,0x17,0x61,0x01,0x00,0x74,0x01,0x00,0x64,0x02,0x00,
  0x6b,0x02,0x00,0x73,0x2f,0x00,0x74,0x01,0x00,0x64,0x03,0x00,0x6b,
  0x02,0x00,0x72,0x39,0x00,0x74,0x02,0x00,0x0b,0x61,0x02,0x00,0x6e,
  0x00,0x00,0x74,0x03,0x00,0x64,0x04,0x00,0x83,0x01,0x00,0x01,0x64,
  0x00,0x00,0x53,0x28,0x05,0x00,0x00,0x00,0x4e,0x69,0x09,0x00,0x00,
  0x00,0x69,0x00,0x00,0x00,0x00,0x69,0xff,0x00,0x00,0x00,0x69,0x1e,
  0x00,0x00,0x00,0x28,0x04,0x00,0x00,0x00,0x75,0x0b,0x00,0x00,0x00,
  0x61,0x6e,0x61,0x6c,0x6f,0x67,0x57,0x72,0x69,0x74,0x65,0x75,0x01,
  0x00,0x00,0x00,0x62,0x75,0x01,0x00,0x00,0x00,0x66,0x75,0x05,0x00,
  0x00,0x00,0x64,0x65,0x6c,0x61,0x79,0x28,0x00,0x00,0x00,0x00,0x28,
  0x00,0x00,0x00,0x00,0x28,0x00,0x00,0x00,0x00,0x75,0x00,0x00,0x00,
  0x00,0x75,0x04,0x00,0x00,0x00,0x6c,0x6f,0x6f,0x70,0x00,0x00,0x00,
  0x00,0x4e,0x28,0x05,0x00,0x00,0x00,0x75,0x07,0x00,0x00,0x00,0x61,
  0x72,0x64,0x75,0x69,0x6e,0x6f,0x75,0x01,0x00,0x00,0x00,0x62,0x75,
  0x01,0x00,0x00,0x00,0x66,0x75,0x05,0x00,0x00,0x00,0x73,0x65,0x74,
  0x75,0x70,0x75,0x04,0x00,0x00,0x00,0x6c,0x6f,0x6f,0x70,0x28,0x00,
  0x00,0x00,0x00,0x28,0x00,0x00,0x00,0x00,0x28,0x00,0x00,0x00,0x00,
  0x75,0x00,0x00,0x00,0x00,0x75,0x08,0x00,0x00,0x00,0x3c,0x6d,0x6f,
  0x64,0x75,0x6c,0x65,0x3e,0x00,0x00,0x00,0x00,0x4e,0x4e
  };

 
#endif
