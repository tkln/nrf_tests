#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "nrf.h"
#include "led.h"
#include "usb_serial.h"
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

void tx_bin(uint8_t c)
{
    int8_t i;
    for (i = 7; i >= 0; --i) {
        if (c & (1<<i)) {
            usb_serial_putchar('1');
        }
        else {
            usb_serial_putchar('0');
        }
    }
    usb_serial_putchar('\n');
    usb_serial_putchar('\r');
}

void serial_nrf_bridge(void)
{
    uint8_t reg_val;
    uint8_t payload;
    int n;
    while (1) {
        /* check nrf fifo not empty flag */
        /* if unset read a byte from the fifo and usb_serial_putchar */
/*        nrf_rx_mode();  */
        nrf_get_reg(REG_FIFO_STATUS, &reg_val);
        if (!(reg_val |= (1<<FIFO_RX_EMPTY))) {
            LED_ON();
            nrf_command_r(R_RX_PAYLOAD, &payload, 1);
            usb_serial_putchar(payload);
        }
        /* if usg getchar >= 0 put the char to nrf tx fifo */
        n = usb_serial_getchar();
        if (n >= 0) {
            LED_ON();
            nrf_command_w(W_TX_PAYLOAD, (uint8_t *)&n, 1);
        }
        nrf_fifo_single_tx();  
        _delay_ms(10);
        LED_OFF();
        nrf_get_reg(REG_FIFO_STATUS, &reg_val);
        tx_bin(reg_val);
    }
}

static int putchar_wrapper(char c, FILE *stream)
{
    if (c == '\n') {
        putchar_wrapper('\r', stream);
    }
    usb_serial_putchar(c);
    return 0;
}

static FILE  mystdout = FDEV_SETUP_STREAM(putchar_wrapper, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
    uint8_t reg_val = 0;
    uint8_t i;
    int n;
    uint8_t payload;
    CPU_PRESCALE(0); /* 16MHz */
    LED_INIT();
    usb_init();
    nrf_init();
    stdout = &mystdout;
    nrf_get_reg(REG_CONFIG, &reg_val);
    
    if (reg_val == (1<<PWR_UP)) {
        LED_ON();
    }
    else {
        LED_OFF();
    }
    
    _delay_ms(200);
    LED_OFF();
      
    return 0;
}
