SOURCES=nrf.c spi.c
PROJECT=nrf
MCU=at90usb1286
F_CPU=16000000

CC=avr-gcc
CFLAGS=-Wall -mmcu=$(MCU)
OBJCOPY=avr-objcopy

PROGRAMMER=

$(PROJECT).out: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(PROJECT).out

$(PROJECT).hex: $(PROJECT).out
	$(OBJCOPY) -j .text -j .data -O ihex $(PROJECT).out $(PROJECT).hex

clean: 
	rm *.out

program: $(PROJECT).hex
	$(PROGRAMMER) 
