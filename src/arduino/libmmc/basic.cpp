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

//#include "basic.h"
#include "../libarduino/Arduino.h"
#include "mmc_lib.h"

extern "C" void __cxa_pure_virtual() { while (1); }

inline void setup (void) {
  
  Serial.begin(115200);
  //byte *buffer = (byte*)malloc(1024);
  int s =0;
  unsigned char mmc_buf[512];
  mmc_init();	//Initializes MMC / SD Card
  mmc_set_blocklen(512);	//Sets blocklen to 512 bytes (default)
  while(s<1024)
  {
  mmc_start_read_block(s, 9);	//start reading sector 1000
  mmc_read_buffer(&mmc_buf[0], 100);
  mmc_read_buffer(&mmc_buf[100], 412);
  for(int i=0;i<512;i++)
  {
    Serial.print((char)mmc_buf[i]);
  }
  s++;
  mmc_stop_read_block();
  }
}
void loop (void) {

}

int main(void) {
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

