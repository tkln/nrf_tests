SOURCES=nrf.c spi.c main.c usb_serial.c
PROJECT=nrf
MCU=at90usb1286
F_CPU=16000000

CC=avr-gcc
CFLAGS=-Wall -mmcu=$(MCU) -Os -Wextra -Wstrict-prototypes 
OBJCOPY=avr-objcopy

AVRDUDE=avrdude
AVRDUDE_PROGRAMMER=avrispmkII
AVRDUDE_PORT=usb

$(PROJECT).out: $(SOURCES) Makefile
	$(CC) $(CFLAGS) $(SOURCES) -o $(PROJECT).out -DF_CPU=$(F_CPU)

$(PROJECT).hex: $(PROJECT).out
	$(OBJCOPY) -j .text -j .data -O ihex $(PROJECT).out $(PROJECT).hex

clean:
	rm *.out

program: $(PROJECT).hex
	$(AVRDUDE) -p $(MCU) -c $(AVRDUDE_PROGRAMMER) -P $(AVRDUDE_PORT) -U flash:w:$(PROJECT).hex:i
