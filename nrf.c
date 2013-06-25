#include "nrf.h"
#include "spi.h"
#include <stddef.h>
#include <util/delay.h>

void nrf_init()
{
    spi_master_init();
    /* init the nrf */
    /*PWR_UP bit in CONFIG, standby-1 */
    _delay_ms(11);
    nrf_set_reg(REG_CONFIG, (1<<PWR_UP));
}

/* len is the amount of data bytes (excluding the command) */
uint8_t nrf_command_r(uint8_t command, uint8_t *data, size_t len)
{
    uint8_t status;
    size_t i;
    /* pull CSN low */
    SPI_PORT &= ~(1<<SPI_SS); 
    /* transmit the command byte */
    spi_master_transmit(command);
    /* fetch back the status register */
    status = spi_master_fetch();
    /* receve len bytes of data to data */
    for (i = 0; i < len; ++i) {
        data[i] = spi_master_receive();
    }
    SPI_PORT |= (1<<SPI_SS);
    return status;
}

/* len is the amount of data bytes (excluding the command) */
uint8_t nrf_command_w(uint8_t command, uint8_t *data, size_t len)
{
    uint8_t status;
    size_t i;
    /* pull CSN low */
    SPI_PORT &= ~(1<<SPI_SS); 
    /* transmit the command byte */
    spi_master_transmit(command);
    /* fetch back the status register */
    status = spi_master_fetch();
    /* receve len bytes of data to data */
    for (i = 0; i < len; ++i) {
        spi_master_transmit(data[i]);
    }
    SPI_PORT |= (1<<SPI_SS);
    return status;
}

uint8_t nrf_set_reg_buf(uint8_t reg, uint8_t *data, size_t len)
{
    /* register is address is 5 bits */
    return nrf_command_w(W_REGISTER & reg, data, len);
}
uint8_t nrf_get_reg_buf(uint8_t reg, uint8_t *data, size_t len)
{
    return nrf_command_r(R_REGISTER & reg, data, len);
}
uint8_t nrf_set_reg(uint8_t reg, uint8_t data)
{
    return nrf_command_w(W_REGISTER & reg, &data, 1);
}
uint8_t nrf_get_reg(uint8_t reg, uint8_t *data)
{
    return nrf_command_r(R_REGISTER & reg, data, 1);
}

