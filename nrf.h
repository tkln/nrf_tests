#ifndef NRF_H
#define NRF_H

#include <stdint.h>
#include <stddef.h>

/* spi commands */
#define R_REGISTER 0x1f /* to be andded with register address */
#define W_REGISTER 0x2f /* same as above */
#define R_RX_PAYLOAD 0x61 
#define W_TX_PAYLOAD 0xa0
#define FLUSH_TX 0xe1
#define FLUSH_RX 0xe2
#define REUSE_TX_PL 0xe3
#define ACTIVATE 0x50
#define R_RX_PL_WID 0x60
#define W_ACK_PAYLOAD 0xa8

/* internal registers */
#define REG_CONFIG 0x00
#define REG_EN_AA 0x01
#define REG_EN_RXADDR 0x02
#define REG_SETUP_AW 0x03
#define REG_SETUP_RETR 0x04
#define REG_RF_CH 0x05
#define REG_RF_SETUP 0x06
#define REG_STATUS 0x07
#define REG_OBSERVE_TX 0x08
#define REG_CD 0x09
#define REG_RX_ADDR_P0 0x0a
#define REG_RX_ADDR_P1 0x0b
#define REG_RX_ADDR_P2 0x0c
#define REG_RX_ADDR_P3 0x0d
#define REG_RX_ADDR_P4 0x0e
#define REG_RX_ADDR_P5 0x0f
#define REG_TX_ADDR 0x10
#define REG_RX_PW_P0 0x11
#define REG_RX_PW_P1 0x12
#define REG_RX_PW_P2 0x13
#define REG_RX_PW_P3 0x14
#define REG_RX_PW_P4 0x15
#define REG_RX_PW_P5 0x16
#define REG_FIFO_STATUS 0x17
#define REG_DYNPD 0x1c
#define REG_FEATURE 0x1d

/* register bits */
/* CONFIG */
#define MASK_RX_DR 6
#define MASK_TX_DS 5
#define MASK_MAX_RT 4
#define EN_CRC 3
#define CRCO 2
#define PWR_UP 1
#define PRIM_RX 0
/* EN_AA */
#define ENAA_P5 5
#define ENAA_P4 4
#define ENAA_P3 3
#define ENAA_P2 2
#define ENAA_P1 1
#define ENAA_P0 0
/* EN_RXADDR */
#define ERX_P5 5
#define ERX_P4 4
#define ERX_P3 3
#define ERX_P2 2
#define ERX_P1 1
#define ERX_P0 0
/* SETUP_AW */
#define AW0 0
#define AW1 1
/* SETUP_RETR */
#define ARD0 4
#define ARD1 5
#define ARD2 6
#define ARD3 7
#define ARC0 0
#define ARC1 1
#define ARC2 2
#define ARC3 3
/* RF_CH */
#define RF_CH 0xef
/* RF_SETUP */
#define PLL_LOCK 4
#define RF_DR 3
#define RF_PWR0 1
#define RF_PWR1 2
#define LNA_HCURR 0
/* STATUS */
#define RX_DR 6
#define TX_DS 5
#define MAX_RT 4
#define RX_P_NO0 1
#define RX_P_NO1 2
#define RX_P_NO2 3
#define TX_FULL 0
/* OBSERVE_TX */
#define PLOS_CNT 0xf0
#define ARC_CNT 0x0f
/* CD */
#define CD 0
/* RX_ADDR_P0:5 */
/* TX_ADDR */
/* RX_PW_P0:5 */
/* FIFO_STATUS */
#define FIFO_TX_REUSE 6
#define FIFO_TX_FULL 5
#define FIFO_TX_EMPTY 4
#define FIFO_RX_FULL 1
#define FIFO_RX_EMPTY 0

#define NRF_CE_PORT PORTB 
#define NRF_CE_DDR DDRB
#define NRF_CE 4

void nrf_init(void);
uint8_t nrf_set_reg_buf(uint8_t reg, uint8_t *data, size_t len);

/* The datasheet has the max lenghts, use a large enough preallocated buffer 
 * for the return value. The actual lenght is placed to the location len 
 * points to.
 */
uint8_t nrf_get_reg_buf(uint8_t reg, uint8_t *data, size_t len);
uint8_t nrf_set_reg(uint8_t reg, uint8_t data);
uint8_t nrf_get_reg(uint8_t reg, uint8_t *data);
/* enable tx mode and empty the fifo */
uint8_t nrf_tx(void);
/* write data to tx payload fifo */
uint8_t nrf_fifo_put(uint8_t *data, size_t len);
uint8_t nrf_command_r(uint8_t command, uint8_t *data, size_t len);
uint8_t nrf_command_w(uint8_t command, uint8_t *data, size_t len);
void nrf_tx_mode(void);
void nrf_rx_mode(void);
void nrf_fifo_single_tx(void);
uint8_t nrf_set_reg_bitmask(uint8_t reg, uint8_t mask);
uint8_t nrf_unset_reg_bitmask(uint8_t reg, uint8_t mask);
#endif
