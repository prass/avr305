AVR305: Half Duplex Compact Software UART
=========================================
I adapted the implementation of Ralph Doncaster's asm version (http://nerdralph.blogspot.de/). I converted it to inline asm and tried to keep as much as possible from Ralph's version to make it more transparent.

It has a really small code size and is perfect for simple debugging over uart.

Not nice coding style but I put all in one header file for easy usage.

The example runs on an attiny45@8MHz and transmits/receives with 115200 Baud.

Configuration
=============
Your can simply change UART_Port, UART_Tx, UART_Rx and also BAUD_RATE in [avr305.h](avr305.h)
