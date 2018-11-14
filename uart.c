/******** uart.c file of C2.5 : UART Driver Code ********/
#include <string.h>
/*** bytes offsets of UART registers from char *base ***/
static char *tab = "0123456789ABCDEF";
#define UDR 0x00
#define UFR 0x18
#define TXFF 0x20
#define RXFE 0x10
typedef volatile struct uart{
	char *base; 						// base address; as char *
	int n; 								// uart number 0-3
}UART;
UART uart; 								// 4 UART structures

int uart_init() 						// UART initialization function
{
	int i; UART *up;
		up = &uart;
		up->base = (char *)(0x20201000);
		up->n = 0;
	
}

int ugetc(UART *up) 						// input a char from UART pointed by up
{
	while (*(up->base+UFR) & RXFE); 		// loop if UFR is RXFE
	return *(up->base+UDR); 				// return a char in UDR
}

int uputc(UART *up, char c) 				// output a char to UART pointed by up
{
	while (*(up->base+UFR) & TXFF); 		// loop if UFR is TXFF
	*(up->base+UDR) = c; 					// write char to data register
}

int ugets(UART *up, char *s) 				// input a string of chars
{
	while ((*s = ugetc(up)) != '\r') {
		uputc(up, *s);
		s++;
	}
	*s = 0;
}

int uprints(UART *up, char *s) 					// output a string of chars
{
	while (*s)
	 uputc(up, *s++);
}

/******************************************************************************/

int urpx(UART *up, int x)
{
  char c;
  if (x){
     c = tab[x % 16];
     urpx(up, x / 16);
  }
  uputc(up, c);
}

int uprintx(UART *up, int x)
{
  uprints(up, "0x");
  if (x==0)
    uputc(up, '0');
  else
    urpx(up, x);
  uputc(up, ' ');
}

int urpu(UART *up, int x)
{
  char c;
  if (x){
     c = tab[x % 10];
     urpu(up, x / 10);
  }
  uputc(up, c);
}

int uprintu(UART *up, int x)
{
  if (x==0)
    uputc(up, '0');
  else
    urpu(up, x);
  uputc(up, ' ');
}

int uprinti(UART *up, int x)
{
  if (x<0){
    uputc(up, '-');
    x = -x;
  }
  uprintu(up, x);
}

int ufprintf(UART *up, char *fmt,...)
{
  int *ip;
  char *cp;
  cp = fmt;
  ip = (int *)&fmt + 1;

  while(*cp){
    if (*cp != '%'){
      uputc(up, *cp);
      if (*cp=='\n')
	uputc(up, '\r');
      cp++;
      continue;
    }
    cp++;
    switch(*cp){
    case 'c': uputc(up, (char)*ip);      break;
    case 's': uprints(up, (char *)*ip);  break;
    case 'd': uprinti(up, *ip);           break;
    case 'u': uprintu(up, *ip);           break;
    case 'x': uprintx(up, *ip);  break;
    }
    cp++; ip++;
  }
}

int uprintf(char *fmt, ...)
{
  int *ip;
  char *cp;
  cp = fmt;
  ip = (int *)&fmt + 1;

  UART *up = &uart;

  while(*cp){
    if (*cp != '%'){
      uputc(up, *cp);
      if (*cp=='\n')
	uputc(up, '\r');
      cp++;
      continue;
    }
    cp++;
    switch(*cp){
    case 'c': uputc(up, (char)*ip);      break;
    case 's': uprints(up, (char *)*ip);   break;
    case 'd': uprinti(up, *ip);           break;
    case 'u': uprintu(up, *ip);           break;
    case 'x': uprintx(up, *ip);  break;
    }
    cp++; ip++;
  }
}

/******************************************************************************/