#include <avr/io.h>
#include <util/delay.h>
#include "nrf.h"
#include "led.h"
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

int main(void)
{
    uint8_t reg_val = 0;
    CPU_PRESCALE(0); /* 16MHz */
    LED_INIT();
    nrf_init();
    nrf_get_reg(REG_CONFIG, &reg_val);
    if (reg_val == ((1<<PWR_UP)|(1<<EN_CRC))) {
        LED_ON();
    }
    else{
        LED_OFF();
    }

    return 0;
}
