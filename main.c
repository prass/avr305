#define F_CPU 8000000UL
#include <util/delay.h>
#include "avr305.h"

int main() {
	char c;
	while(1) {
		while(c=readc()) {
			printc(c);
		}
	}
}
