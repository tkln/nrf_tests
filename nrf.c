#include "nrf.h"
#include "spi.h"
#include <stddef.h>
#include <util/delay.h>

void nrf_init(void)
{
    spi_master_init();
    /* init the nrf */
    /*PWR_UP bit in CONFIG, standby-1 */
    NRF_CE_PORT &= ~(1<<NRF_CE);
    NRF_CE_DDR |= (1<<NRF_CE);
    _delay_ms(11);
    nrf_set_reg_bitmask(REG_CONFIG, (1<<PWR_UP));
    nrf_set_reg(REG_RX_PW_P0, 1);
}

/* len is the amount of data bytes (excluding the command) */
uint8_t nrf_command_r(uint8_t command, uint8_t *data, size_t len)
{
    uint8_t status;
    size_t i;
    /* pull CSN low */
    SPI_PORT &= ~(1<<SPI_SS); 
    /* transmit the command byte */
    spi_data_order_msb();
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
    spi_data_order_msb();
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
    return nrf_command_w((0x1f & reg) | (1<<5), data, len);
}
uint8_t nrf_get_reg_buf(uint8_t reg, uint8_t *data, size_t len)
{
    return nrf_command_r(R_REGISTER & reg, data, len);
}
uint8_t nrf_set_reg(uint8_t reg, uint8_t data)
{
    return nrf_command_w((0x1f & reg) | (1<<5), &data, 1);
}
uint8_t nrf_get_reg(uint8_t reg, uint8_t *data)
{
    return nrf_command_r(R_REGISTER & reg, data, 1);
}
void nrf_tx_mode(void)
{
    nrf_set_reg_bitmask(REG_CONFIG, (1<<PWR_UP));
    nrf_unset_reg_bitmask(REG_CONFIG, (1<<PRIM_RX));
    NRF_CE_PORT |= (1<<NRF_CE);
}
void nrf_rx_mode(void)
{
    nrf_set_reg_bitmask(REG_CONFIG, (1<<PWR_UP) | (1<<PRIM_RX));
}
void nrf_fifo_single_tx(void)
{
    nrf_set_reg_bitmask(REG_CONFIG, (1<<PWR_UP));
    nrf_unset_reg_bitmask(REG_CONFIG, (1<<PRIM_RX));
    _delay_ms(10);
    NRF_CE_PORT |= (1<<NRF_CE);
    _delay_us(20);
    NRF_CE_PORT &= ~(1<<NRF_CE);
    _delay_ms(10);
}
uint8_t nrf_set_reg_bitmask(uint8_t reg, uint8_t mask)
{
    uint8_t reg_val;
    nrf_get_reg(reg, &reg_val);
    reg_val |= mask;
    return nrf_set_reg(reg, reg_val);
}

uint8_t nrf_unset_reg_bitmask(uint8_t reg, uint8_t mask)
{
    uint8_t reg_val;
    nrf_get_reg(reg, &reg_val);
    reg_val &= ~(mask);
    return nrf_set_reg(reg, reg_val);
}

