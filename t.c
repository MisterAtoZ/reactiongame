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
	enable_GPLEN(&gpio[20]);
	enable_GPLEN(&gpio[26]);
	
	
	
	set_pull(20,2);
	set_pull(26,2);
	
	int i;
	char string[64];
	UART *up;
	uart_init(); // initialize UARTs
	up = &uart; // test UART0
	uprintf("Enter lines from serial terminal 0\n\r");
	while(1)
	{
		set_output_pin(&gpio[21]);
		
		volatile unsigned int tim;
		for(tim = 0; tim < 5000000; tim++);
		
		clear_pin(&gpio[21]);
		
		uprintf("de waarde die uit de knop komt is: ");
		uprintf("\n\r");
		
		clear_event_detect(&gpio[20]);
		clear_event_detect(&gpio[26]);
		int knop1test = getChange(&gpio[20]);
		int knop2test = getChange(&gpio[26]);
		uprintf("de waarde die uit de knop komt is: %d", knop1test);
		uprintf("\n\r");
		uprintf("de waarde die uit de knop komt is: %d", knop2test);
		uprintf("\n\r");
		
		uprintf("de waarde van event detect: %d", gpio_reg[GPIO_GPEDS0]);
		uprintf("\n\r");		
		
		
		int eruit = 0;
		
		while(eruit == 0) {
			int knop1 = getChange(&gpio[20]);
			int knop2 = getChange(&gpio[26]);
			uprintf("de waarde die uit de knop komt is: %d", knop1);
			uprintf("\n\r");
			uprintf("de waarde die uit de knop komt is: %d", knop2);
			uprintf("\n\r");
			if(knop1 == 1) {
				uprintf("de waarde die uit de knop komt is: %d", knop1);
				uprintf("\n\r");
				uprintf("Speler 1 heeft gewonnen");
				uprintf("\n\r");
				eruit = 1;
			}
			else if(knop2 == 1) {
				uprintf("de waarde die uit de knop komt is: %d", knop2);
				uprintf("\n\r");
				uprintf("Speler 2 heeft gewonnen");
				uprintf("\n\r");
				eruit = 1;
			}
		}
		
		ugets(up, string);
		uprintf("tis gedaan");
		uprintf(string);
		uprintf("\n\r");
		if (strcmp(string, "end")==0)
			break;
		
	}
	
	uprintf("Compute sum of array:\n\r");
	sum = 0;
	for (i=0; i<10; i++) sum += v[i];
	uprintf("sum = ");
	uputc(up, (sum/10)+'0'); uputc(up, (sum%10)+'0');
	uprintf("\n\rEND OF RUN\n\r");
}