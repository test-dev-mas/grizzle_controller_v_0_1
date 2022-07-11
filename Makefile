CC = /home/dev/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-gcc
CC_OBJCOPY = /home/dev/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-objcopy
CC_UPLOAD = /home/dev/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin/avrdude
CONF = /home/dev/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/etc/avrdude.conf
CFLAGS = -Os -DF_CPU=16000000UL -mmcu=atmega2560

USBPORT = /dev/ttyUSB1

all: main.out

main.out: main.o spi.o timers.o uart.o
	$(CC) $(CFLAGS) main.o spi.o timers.o uart.o -o main.out

%.o: %.c
	$(CC) $(CFLAGS) -c $^

main.hex: main.out
	$(CC_OBJCOPY) -O ihex -R .eeprom $< $@

# an Arduino Uno is used as the programmer
install.main: main.hex
	$(CC_UPLOAD) -v -V -C $(CONF) -D -p atmega2560 -c wiring -P $(USBPORT)  -b 115200 -U flash:w:$<

clean:
	rm *.o *.out *.hex