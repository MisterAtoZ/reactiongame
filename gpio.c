/*****gpio.c*****/
#include "defines.h"
volatile unsigned int* gpio_reg;

typedef volatile struct gpio{
int sel;
int bit;
int set;
int clr;
int gpio_bit;
int event_detect;
int ren;
int hen;
int len;
int pud;
}GPIO;

GPIO gpio[27];

void gpio_init(){
	gpio_reg = (unsigned int*)GPIO_BASE;
	int i;
	int tiental;
	GPIO *gp;
	for(i = 0; i<=27;i++){
		gp = &gpio[i];
		gp->bit = (i%10)*3;
		gp->set = GPIO_GPSET0;
		gp->clr = GPIO_GPCLR0;
		gp->ren = GPIO_GPREN0;
		gp->hen = GPIO_GPHEN0;
		gp->len = GPIO_GPLEN0;
		gp->pud = GPIO_GPPUD;
		gp->event_detect = GPIO_GPEDS0;
		gp->gpio_bit = i;
		tiental = i/10;
		switch (tiental){
			case 0:
				gp->sel = GPIO_GPFSEL0;
				break;
			case 1:
				gp->sel = GPIO_GPFSEL1;
				break;
			case 2:
				gp->sel = GPIO_GPFSEL2;
				break;
			default: break;
		}
	}
}

void clear_event_detect(GPIO *up){
	gpio_reg[up->event_detect] = (1 << up->gpio_bit);
}


int getChange(GPIO *up) {
	
	int events_detected = gpio_reg[up->event_detect];
	int mask = (1<< up->gpio_bit);
	
	int resultaat = events_detected & mask;
	clear_event_detect(up); 
	
	return (resultaat > up->gpio_bit);
}


void set_GPIO_output(GPIO *up){
	gpio_reg[up->sel] |= (1 << up->bit);
}

void set_GPIO_input(GPIO *up){
	gpio_reg[up->sel] |= (0 << up->bit);
}

void set_GPIO_ALT0(GPIO *up){
	gpio_reg[up->sel] |= (4 << up->bit);
}

void set_output_pin(GPIO *up){
	gpio_reg[up->set] = (1 << up->gpio_bit);
}

void clear_pin(GPIO *up){
	gpio_reg[up->clr] = (1 << up->gpio_bit);
}

void enable_GPREN(GPIO *up){ //enable the rising edge detection
	gpio_reg[up->ren] |= (1 << up->gpio_bit);
}

void enable_GPHEN(GPIO *up){ //high level enable detect
	gpio_reg[up->hen] |= (1 <<up->gpio_bit);
}

void enable_GPLEN(GPIO *up){ //low level enable detect
	gpio_reg[up->len] |= (1 <<up->gpio_bit);
}
/*
void enable_GPEDSn(GPIO *up){ //Event Detect Status Registers
	gpio_reg[up->eds] |= (1 <<up->gpio_bit);
}
*/

void set_pullup(GPIO *up){
	gpio_reg[GPIO_GPPUD] =2;
}

void dissable_pulldown(GPIO *up) { //niet nodig want kan altijd aan blijven
	gpio_reg[up->pud] |= (0 << up->gpio_bit);
}

void set_pull(int gpio, int pull){
	volatile unsigned int tim;
	int move = gpio % 32;
	
	gpio_reg[GPIO_GPPUD] = pull & 0x03;
	for(tim = 0; tim < 150; tim++);
	if(gpio<32)
		gpio_reg[GPIO_GPPUDCLK0] = 1 << move;
	else
		gpio_reg[GPIO_GPPUDCLK1] = 1 << move;
	for(tim = 0; tim < 150; tim++);
	if(gpio<32)
		gpio_reg[GPIO_GPPUDCLK0] = 0;
	else
		gpio_reg[GPIO_GPPUDCLK1] = 0;
}