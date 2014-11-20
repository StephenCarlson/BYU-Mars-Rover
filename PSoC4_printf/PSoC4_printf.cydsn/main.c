/* ========================================
 *
  The following firmware in main.c was developed by Cypress Semiconductor
  This work is licensed under a Creative Commons Attribution 3.0 Unported License.
  
  http://creativecommons.org/licenses/by/3.0/deed.en_US
  
  You are free to:
  -To Share — to copy, distribute and transmit the work 
  -To Remix — to adapt the work 
  -To make commercial use of the work
  
  The tinyprintf of code is Copyright (c) 2004,2012 Kustaa Nyholm / SpareTimeLabs.
  You can find the source code at http://www.sparetimelabs.com/tinyprintf/tinyprintf.php.
  

 * ========================================
*/

#include <device.h>

#define CR 0x0D
#define LF 0x0A

/* Used to insert a carriage return and line feed */
#define NewLine() UART_UartPutChar(CR); UART_UartPutChar(LF);

/* For use with tinyprintf function */
/* defines how the function will transmit data */
void putdata ( void* p, char c){
	if(c=='\n') UART_UartPutChar('\r');
	UART_UartPutChar(c);
}


void main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
	
	/* Start SCB UART module */
	UART_Start();
	/* initialize tinyprintf with PSoC putchar function */
	init_printf(NULL,putdata);

    CyGlobalIntEnable; /* Uncomment this line to enable global interrupts. */
	
	/* tinyprintf supports the following formats:
	
	'd' int as a signed decimal number
	'u' uint as a decimal number
	'c' character
	's' null terminated string
	'x' / 'X' hexadecimal with lower / upper case letters
	
	*/
	
	int8 d = -10;
	uint8 u = 127;
	char c = 'c';
	const char s[] = "Null String\n";
	char buffer[50];

	/* Place your application code here. */

	UART_UartPutString("USB_UART\n");
	NewLine();
	NewLine();

	/* printf examples */

	UART_UartPutString("printf Examples");
	NewLine();
	NewLine();

	/* print variables using printf */
	printf("%d", d);
	NewLine();
	printf("%u", u);
	NewLine();
	printf("%x", u);
	NewLine();
	printf("%X", u);
	NewLine();
	printf("%c", c);
	NewLine();
	printf("%s", s);
	NewLine();
	
	/* sprintf examples */
	
	UART_UartPutString("sprintf Examples");
	NewLine();
	NewLine();
	
	sprintf(buffer, "signed decimal: %d",d);
	UART_UartPutString(buffer);
	NewLine();
	
	sprintf(buffer, "unsigned decimal: %u",u);
	UART_UartPutString(buffer);
	NewLine();
	
	sprintf(buffer, "lower case hex: %x",u);
	UART_UartPutString(buffer);
	NewLine();
	
	sprintf(buffer, "uppper case hex: %X",u);
	UART_UartPutString(buffer);
	NewLine();
	
	sprintf(buffer, "character: %c",c);
	UART_UartPutString(buffer);
	NewLine();
	
	sprintf(buffer, "string: %s",s);
	UART_UartPutString(buffer);
	
    for(;;)
    {

    }
}

/* [] END OF FILE */
