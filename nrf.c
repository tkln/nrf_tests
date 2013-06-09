#include "nrf.h"
#include "spi.h"
#include <stddef.h>

void nrf_init()
{
    /* init pin modes and spi */
    /* init the nrf */
}

void nrf_set_reg(uint8_t reg, uint8_t *data, uint8_t len)
{
    /* spi send */
}

uint8_t *nrf_get_reg(uint8_t reg, uint8_t *len)
{
    return NULL;
}
