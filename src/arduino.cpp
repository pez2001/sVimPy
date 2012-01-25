#include "arduino.h"

//#include "arduino/test_pyc.h"

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

void AddInternalFunctions(vm *vm)
{
	function_object *list = CreateCFunction(&if_list, "list");
	function_object *range = CreateCFunction(&if_range, "range");
	function_object *print = CreateCFunction(&if_print, "print");
	function_object *sum = CreateCFunction(&if_sum, "sum");
	function_object *next = CreateCFunction(&if_next, "next");
	vm_AddFunctionObject(vm, list);
	vm_AddFunctionObject(vm, range);
	vm_AddFunctionObject(vm, print);
	vm_AddFunctionObject(vm, sum);
	vm_AddFunctionObject(vm, next);

	function_object *cc = CreateCFunction(&custom_code, "custom_code");
	vm_AddFunctionObject(vm, cc);

}



inline void setup (void) {
  
 Serial.begin(115200);
//  delay(250);
 Serial.print("hi");
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


	//ptr_tests();
	vm *vm = vm_Init(NULL);
	AddInternalFunctions(vm);
	vm_Close(vm);

 Serial.print("done");

}
void loop (void) {
/*  digitalWrite(13, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(13, LOW);    // set the LED off
  delay(100);              // wait for a second
 */
 Serial.print("loop\n");
}
int main(void) {
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

