/************* t.c file of C2.5 **************/
int v[] = {1,2,3,4,5,6,7,8,9,10}; // data array
int sum;
#include "uart.c" // UART driver code file
#include "gpio.c"
int main()
{
	gpio_init();
	set_GPIO_ALT0(&gpio[14]);
	set_GPIO_ALT0(&gpio[15]);
	set_GPIO_output(&gpio[21]);
	set_GPIO_input(&gpio[20]);
	set_GPIO_input(&gpio[26]);
	
	enable_GPREN(&gpio[20]);
	enable_GPREN(&gpio[26]);
	enable_GPHEN(&gpio[20]);
	enable_GPHEN(&gpio[26]);
	
	set_pulldown(&gpio[20]);
	set_pulldown(&gpio[26]);
	
	int i;
	char string[64];
	//UART *up;
	//uart_init(); // initialize UARTs
	//up = &uart[0]; // test UART0
	uprints("Enter lines from serial terminal 0\n\r");
	while(1)
	{
		set_output_pin(&gpio[21]);
		
		volatile unsigned int tim;
		for(tim = 0; tim < 5000000; tim++);
		
		clear_pin(&gpio[21]);
		
		uprints("de waarde die uit de knop komt is: ");
		uprints("\n\r");
		
		int eruit = 0;
		
		while(eruit == 0) {
			int knop1 = getChange(&gpio[20]);
			int knop2 = getChange(&gpio[26]);
			
			if(knop1 == 0) {
				//uprints("de waarde die uit de knop komt is: "+ knop1);
				uprints("\n\r");
				uprints("Speler 1 heeft gewonnen");
				uprints("\n\r");
				eruit = 1;
			}
			else if(knop2 == 1) {
				uprints("Speler 2 heeft gewonnen");
				uprints("\n\r");
				eruit = 1;
			}
		}
		
		ugets(string);
		uprints(" ");
		uprints(string);
		uprints("\n\r");
		if (strcmp(string, "end")==0)
			break;
		
	}
	
	uprints("Compute sum of array:\n\r");
	sum = 0;
	for (i=0; i<10; i++) sum += v[i];
	uprints("sum = ");
	uputc((sum/10)+'0'); uputc((sum%10)+'0');
	uprints("\n\rEND OF RUN\n\r");
}