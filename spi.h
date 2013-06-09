#include <avr/io.h>

#ifndef SPI_H
#define SPI_H
/* hw pin defs */
#define SPI_DDR DDRB

#define SPI_MISO 3
#define SPI_MOSI 2
#define SPI_SCLK 1 /* SCK on the nRF */
#define SPI_SS 0 /* CSN on the nRF */

void spi_master_init();
void spi_master_transmit(uint8_t data);
#endif
