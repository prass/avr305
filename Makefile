TARGET = avr305
MCU = attiny45

CC = avr-gcc
CFLAGS =-Os -mmcu=$(MCU)

$(TARGET).hex: $(TARGET)
	avr-objcopy -j .text -j .data -O ihex $< $@
$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ $^
clean:
	-rm $(TARGET)
	-rm $(TARGET).hex
.PHONY: clean
