#include <avr/io.h>

//Set tx to PB2 / rx to PB1
#define UART_Port _SFR_IO_ADDR(PORTB)
#define UART_Tx 2
#define UART_Rx 1

//Define baudrate
#define BAUD_RATE 115200

//Calculate delays
#ifdef F_CPU
#define TXDELAY (((F_CPU/BAUD_RATE)-7 +1.5)/3)
#define RXDELAY (((F_CPU/BAUD_RATE)-5 +1.5)/3)
#else
  #error CPU frequency F_CPU undefined
#endif

void printc(char ch) {
	uint8_t txdelay = TXDELAY;
	uint8_t delayCount = 0;

	__asm__ __volatile__(
		"0: cli\n\t"
		"   sbi %[uart_port]-1,%[uart_tx]\n\t"
		"   cbi %[uart_port],%[uart_tx]\n\t"
		"   in __tmp_reg__,%[uart_port]\n\t"
		"   ldi r25,3\n\t"
		"1: mov %[delayCount],%[txdelay]\n\t"
		"2: dec %[delayCount]\n\t"
		"   brne 2b\n\t"
		"   bst %[ch],0\n\t"
		"   bld __tmp_reg__,%[uart_tx]\n\t"
		"   lsr r25\n\t"
		"   ror %[ch]\n\t"
		"   out %[uart_port],__tmp_reg__\n\t"
		"   brne 1b\n\t"
		:
		  [ch] "+r" (ch),
		  [delayCount] "+r" (delayCount)
		:
		  [uart_port] "M" (UART_Port),
		  [uart_tx] "M" (UART_Tx),
		  [txdelay] "r" (txdelay)
		:
		  "r25"
	);
}

void println(const char *string) {
	while(*string) {
		printc(*string++);
	}
	printc('\n');
	printc('\r');
}

char readc() {
	uint8_t rxdelay = RXDELAY;
	uint8_t rxdelay15 = (RXDELAY*1.5)-2.5;
	uint8_t delayCount = 0;

	__asm__ __volatile__(
		"0: cbi %[uart_port]-1,%[uart_rx]\n\t"
		"   sbi %[uart_port],%[uart_rx]\n\t"
		"   mov %[delayCount],%[rxdelay15]\n\t"
		"   ldi %[rxdelay15],0x80\n\t"
		"1: sbic %[uart_port]-2,%[uart_rx]\n\t"
		"   rjmp 1b\n\t"
		"   cli\n\t"
		"2: subi %[delayCount], 1\n\t"
		"   brne 2b\n\t"
		"   mov %[delayCount],%[rxdelay]\n\t"
		"   sbic %[uart_port]-2,%[uart_rx]\n\t"
		"   sec\n\t"
		"   ror %[rxdelay15]\n\t"
		"   brcc 2b\n\t"
		"3: dec %[delayCount]\n\t"
		"   brne 3b\n\t"
	:
	  [rxdelay15] "+r" (rxdelay15),
	  [delayCount] "+r" (delayCount)
	:
	  [uart_port] "M" (UART_Port),
	  [uart_rx] "M" (UART_Rx),
	  [rxdelay] "r" (rxdelay)
	);
	return rxdelay15;
}
