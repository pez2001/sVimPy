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

#include "arduino.h"



extern "C" void __cxa_pure_virtual() { while (1); }

#define UART_BAUD_SELECT  115200

void put_char(unsigned char c) 
{ 
        while ( (UCSR0A & _BV(UDRE0)) ==0); 
        //if (c =='\n') 
        //    put_char ('\r'); 
        UDR0 = c; 
} 


void initUSART (void) 
{ 
     UCSR0B = ( _BV(RXEN0) | _BV(TXEN0) );    
	 unsigned short baud_setting = (F_CPU / 8 / UART_BAUD_SELECT - 1) / 2;
     UBRR0H =  (uint8_t) (baud_setting >>8); 
     UBRR0L = (uint8_t) (baud_setting);    
} 

//int debug_level = 0;

extern void *__bss_end;
extern void *__brkval;

int get_free_memory(void)
{
  int free_memory;

  if((int)__brkval == 0)
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  else
    free_memory = ((int)&free_memory) - ((int)__brkval);

  return(free_memory);
}


static FILE uartout = {0} ;

static int uart_putchar (char c, FILE *stream)
{
    //Serial.write(c) ;
    put_char(c);
	return(0);
}


int __attribute__((OS_main)) main(void)  
{
	init();
	//pinMode(13,OUTPUT);
	//digitalWrite(13,LOW);

	//#if defined(USBCON)
	//USB.attach();
	//#endif
	initUSART();
	fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &uartout;
	printf("in:%4d\r\n",get_free_memory());
	//debug_level = 0;
	vm *vm = vm_Init(NULL);
	AddArduinoGlobal(vm);
		//setup(vm);
	//debug_level |= DEBUG_VERBOSE_TESTS;	
	//debug_level |= DEBUG_SHOW_OPCODES;
	//debug_printf(DEBUG_ALL,"\nsetup:%d\r\n",get_free_memory());
	//ptr_tests();
	//Serial.println("");
	//Serial.println("setup");
	//printf("si:%4d\r\n",get_free_memory());
	//delay(1000);	
	//AddInternalFunctions(vm);
	//printf("if:%4d\r\n",get_free_memory());
	//delay(1000);
	printf("ag:%4d\r\n",get_free_memory());
	//delay(1000);
	//	digitalWrite(13,HIGH);

	//Serial.println(get_free_memory());
	//printf("ag:%4d\r\n",get_free_memory());
	//delay(1000);
	//stream *m = stream_CreateFromFlashBytes(((char*)&blink),BLINK_LEN);
	//stream *m = stream_CreateFromFlashBytes(((char*)&fade_min),FADE_MIN_LEN);
	stream *m = stream_CreateFromBytes(((char*)&fade_min),FADE_MIN_LEN);
	//stream *m = stream_CreateFromFlashBytes(((char*)&blink),BLINK_LEN);
	//Serial.println("stream");
	//Serial.println(get_free_memory());
	//printf("cs:%4d\n",get_free_memory());
	//delay(1000);
	//debug_printf(DEBUG_ALL,"run pyc\r\n");
	//Serial.print("run");
	
	vm_RunRPYC(vm,m,0);
	printf("rp:%4d\r\n",get_free_memory());
	//debug_printf(DEBUG_ALL,"run thru\r\n");
	//Serial.println("thru");
	vm_RunFunction(vm,"setup",NULL,NULL);
	printf("rf:%4d\r\n",get_free_memory());
	//debug_printf(DEBUG_ALL,"called setup\r\n");
	//vm_Close(vm);
	//Serial.println("done");
	 //pinMode(13, OUTPUT);     
    int c = 0;
	for (;;)
	{
		//digitalWrite(13,HIGH);
		
		vm_RunFunction(vm,"loop",NULL,NULL);
		//delay(500);
		//digitalWrite(13,LOW);
		//delay(500);
		//printf("loop:%4d\r\n",c);
		c++;
		printf("loop 	:%4d ,%d\r\n",get_free_memory(),c);
		int ms = 64;
		for(char l = 1; l<5;l++)
		{
			char *m = (char*)malloc(ms);
			if(m == NULL)
				printf("warning max chunk size reached: %d\r\n",ms);
			else
				free(m);
			ms = ms * 2;
		}
		}
		//		loop(vm);
        
	return 0;
}

