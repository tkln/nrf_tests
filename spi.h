/* TODO finnish interrupt driven systems */
#include <avr/io.h>

#ifndef SPI_H
#define SPI_H
/* hw pin defs */
#define SPI_DDR DDRB
#define SPI_PORT PORTB

#define SPI_MISO 3
#define SPI_MOSI 2
#define SPI_SCLK 1 /* SCK on the nRF */
#define SPI_SS 0 /* CSN on the nRF */

#ifndef SPI_TX_FIFO_SIZE
#define SPI_TX_FIFO_SIZE 16
#endif

#ifndef SPI_RX_FIFO_SIZE
#define SPI_RX_FIFO_SIZE 16
#endif

/* Initaliazes the SPI hardware. */
void spi_master_init(void);
void spi_master_transmit(uint8_t data);
uint8_t spi_master_fetch(void);
uint8_t spi_master_receive(void);

/* Puts the byte into transmit FIFO.
 * Starts the transmit process, if not already running.
 * If the FIFO is full, it will block until the has byte has been placed.
 */
int spi_master_transmit_fifo_put(uint8_t data);

/* Starts the transmit process, if not already runnung. */
void spi_master_transmit_fifo_flush(void);

/* Starts the transmit process, if not already rurnning.
 * Blocks until the FIFO is empty. */
void spi_master_transmit_fifo_wait(void);

void spi_data_order_lsb(void);
void spi_data_order_msb(void);
#endif
