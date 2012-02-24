#include "arduino.h"



extern "C" void __cxa_pure_virtual() { while (1); }

#define UART_BAUD_SELECT  115200

void put_char(unsigned char C) 
{ 
        while ( (UCSR0A & _BV(UDRE0)) ==0); 
        if (C =='\n') 
            putchar ('\r'); 
        UDR0 = C; 
} 


//initialise USART: stop bits: 1, parity: none 
void initUSART (void) 
{ 
     UCSR0B = ( _BV(RXEN0) | _BV(TXEN0) );    
     UBRR0H =  (uint8_t) (UART_BAUD_SELECT >>8); 
     UBRR0L = (uint8_t) (UART_BAUD_SELECT);    
} 

/*void ptr_tests()
{
	ptr_list *p = ptr_CreateList(0, 0);
	digitalWrite(13, HIGH);
	delay(1000);
	ptr_Push(p, (void*)1);
	ptr_Push(p, (void*)2);
	ptr_Push(p, (void*)3);
	ptr_Push(p, (void*)4);
	digitalWrite(13, LOW);
	delay(1000);
	long x =(long) ptr_Pop(p);
	ptr_Push(p, (void*)5);
	digitalWrite(13, HIGH);
	delay(1000);
	long r = (long)ptr_Remove(p, 2);
	digitalWrite(13, LOW);
	delay(1000);
	digitalWrite(13, HIGH);
	delay(1000);
	ptr_Insert(p, 3, (void*)66);
	ptr_Insert(p, 0, (void*)99);
	digitalWrite(13, HIGH);
	delay(1000);
	ptr_Set(p, 1, (void*)22);
	digitalWrite(13, LOW);
	delay(1000);
	ptr_Queue(p, (void*)88);
	digitalWrite(13, HIGH);
	delay(1000);
	ptr_Clear(p);
	digitalWrite(13, LOW);
	delay(1000);
	ptr_Push(p, (void*)0);
	digitalWrite(13, HIGH);
	delay(1000);
	for (int i = 0; i < 100; i++)
		ptr_Push(p, (void*)1);
	for (int i = 0; i < 100; i++)
		ptr_Pop(p);

	for (int i = 0; i < 100; i++)
	{
		ptr_Push(p,(void*)0);
		ptr_Pop(p);
	}
	digitalWrite(13, LOW);
	delay(1000);
	ptr_CloseList(p);
}
*/

int debug_level = 0;

extern void *__bss_end;
extern void *__brkval;

int get_free_memory(void)
{
  int free_memory;

  if((int)__brkval == 0)
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  else
    free_memory = ((int)&free_memory) - ((int)__brkval);

  return free_memory;
}

inline void setup (vm *vm) 
{
  
	Serial.begin(115200);
	//debug_printf(DEBUG_ALL,"\r\nsetup\r\n");
//  delay(250);
//initUSART();
//  pinMode(13, OUTPUT);     
//  digitalWrite(13, HIGH);
//  delay(1000);              // wait for a second
//  digitalWrite(13, LOW);
 //ptr_tests();
//put_char('H');
//put_char('i');
//put_char('\n');

//for(int i = 0;i<TEST_PYC_LEN;i++)
//	Serial.print(test_pyc[i]);
//	put_char((unsigned char)test_pyc[i]);
	debug_level = 0;
	debug_level |= DEBUG_VERBOSE_TESTS;	
	debug_level |= DEBUG_SHOW_OPCODES;
	debug_printf(DEBUG_ALL,"setup:%d\r\n",get_free_memory());
	//ptr_tests();
	//Serial.print("setup");
	AddInternalFunctions(vm);
	AddArduinoFunctions(vm);
	AddArduinoGlobals(vm);
	//Serial.println(get_free_memory());
	stream *m = stream_CreateFromFlashBytes(((char*)&blink),BLINK_LEN);
	//Serial.print("stream");
	//Serial.println(get_free_memory());
	//debug_printf(DEBUG_ALL,"created stream:%d\r\n",get_free_memory());
	//debug_printf(DEBUG_ALL,"run pyc\r\n");
	//Serial.print("run");
	vm_RunPYC(vm,m,0);
	//debug_printf(DEBUG_ALL,"run thru\r\n");
	//Serial.println("thru");
	//vm_CallFunction(vm,"setup",NULL,0);
	//vm_Close(vm);
	//Serial.print("done");

}
void loop (vm *vm) 
{
/*  digitalWrite(13, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(13, LOW);    // set the LED off
  delay(100);              // wait for a second
 */
 //Serial.print("loop\n");	
 //debug_printf(DEBUG_ALL,"loop\n");
	//vm_CallFunction(vm,"loop",NULL,0);
}
int __attribute__((OS_main)) main(void)  
{
	init();

	#if defined(USBCON)
	USB.attach();
	#endif
	
	vm *vm = vm_Init(NULL);

	setup(vm);
    
	for (;;)
		loop(vm);
        
	return 0;
}

