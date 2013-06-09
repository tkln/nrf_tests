#include "spi.h"

void spi_master_init()
{
    /* set mosi and sclk pins as outputs */
    SPI_DDR = (1<<SPI_MOSI)|(1<<SPI_SCLK);
    /* enable spi, set clock rate fcpu/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void spi_master_transmit(uint8_t data)
{
    SPDR = data;
    while(!(SPSR & (1<<SPIF)))
        ;
}
