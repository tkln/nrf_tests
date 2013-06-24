#include "spi.h"
#include <stddef.h>
#include <stdbool.h>

volatile uint8_t spi_tx_fifo[SPI_TX_FIFO_SIZE];
volatile size_t spi_tx_fifo_data_start;
volatile size_t spi_tx_fifo_data_end;
volatile uint8_t spi_rx_fifo[SPI_RX_FIFO_SIZE];
volatile bool transmitting;

/* if spi_tx_fifo_data_start == spi_tx_fifo_data_end and not transmitting:
 * the buffer is empty, if transmitting the buffer is full.
 */

void spi_master_init(void)
{
    /* set mosi, sclk and ss pins as outputs */
    SPI_DDR = (1<<SPI_MOSI)|(1<<SPI_SCLK)|(1<<SPI_SS);
    SPI_PORT |= (1<<SPI_SS);
    /* enable spi, set clock rate fcpu/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void spi_master_transmit(uint8_t data)
{
    SPDR = data;
    while (!(SPSR & (1<<SPIF)))
        ;
}

uint8_t spi_master_fetch(void)
{
    while (!(SPSR & (1<<SPIF)))
        ;
    return SPDR;
}

uint8_t spi_master_receive(void)
{
    spi_master_transmit(0x00); /* swing the clock line around */
    return spi_master_fetch();
}


/* to be finished */

void spi_master_fifo_start_transmit(void)
{
    /* transmit the first byte and increment the counter to the next byte */
    if (spi_tx_fifo_data_start != spi_tx_fifo_data_end) {
        transmitting = true; 
        SPDR = spi_tx_fifo[(spi_tx_fifo_data_start++) % SPI_TX_FIFO_SIZE];
    }
    else {
        transmitting = false;
    }
}

void spi_fifo_mastre_transmit_flush(void)
{
    /* checks if transmitting already */
    if (!transmitting) {
        spi_master_fifo_start_transmit();
    }
    while (transmitting) {
        /* wait for the interrupt handlers to clear the buffer */
    }
}

int spi_master_transmit_fifo_put(uint8_t data)
{
    /* increment fifo end. wrap to begining if reached end */
    /*size_t new_element_id;*/    /* wait until there is space in the fifo */
    while (spi_tx_fifo_data_start == spi_tx_fifo_data_end && transmitting) {
    }
    spi_tx_fifo[spi_tx_fifo_data_end] = data;
    
    spi_master_fifo_start_transmit();
    return 0;
}
