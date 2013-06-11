#include "spi.h"
#include <stddef.h>
#include <stdbool.h>

volatile uint8_t spi_fifo[SPI_FIFO_SIZE];
volatile size_t spi_tr_fifo_data_start;
volatile size_t spi_tr_fifo_data_end;
volatile bool transmitting;

/* if spi_tr_fifo_data_start == spi_tr_fifo_data_end and not transmitting:
 * the buffer is empty, if transmitting the buffer is full.
 */

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
    while (!(SPSR & (1<<SPIF)))
        ;
}

void spi_master_fifo_start_transmit()
{
    /* transmit the first byte and increment the counter to the next byte */
    if (spi_tr_fifo_data_start != spi_tr_fifo_data_end) {
        transmitting = true; 
        SPDR = spi_fifo[(spi_tr_fifo_data_start++) % SPI_FIFO_SIZE];
    }
    else {
        transmitting = false;
    }
}

void spi_fifo_mastre_transmit_flush()
{
    /* checks if transmitting already */
    if (!transmitting) {
        spi_master_fifo_transmit
    }
    while (transmitting) {
        /* wait for the interrupt handlers to clear the buffer */
    }
}

int spi_master_transmit_fifo_put(uint8_t data)
{
    /* increment fifo end. wrap to begining if reached end */
    size_t new_element_id = (++spi_tr_fifo_data_end % SPI_FIFO_SIZE);
    /* wait until there is space in the fifo */
    while (spi_tr_fifo_data_start == spi_tr_fifo_data_end && transmitting) {
    }
    spi_fifo[new_elemnt_id] = data;
    spi_master_fifo_start_transmit();
}
