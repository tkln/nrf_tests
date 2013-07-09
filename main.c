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

void pwr_up_test(void)
{
    uint8_t reg_val;

    nrf_get_reg(REG_CONFIG, &reg_val);
    
    if (reg_val == (1<<PWR_UP)) {
        LED_ON();
    }
    else {
        LED_OFF();
    }
    
    _delay_ms(200);
    LED_OFF();
} 

void print_regs (void)
{
    uint8_t config, status, fifo_status, obs, cd;
    nrf_get_reg(REG_CONFIG, &config);
    nrf_get_reg(REG_STATUS, &status);
    nrf_get_reg(REG_FIFO_STATUS, &fifo_status);
    nrf_get_reg(REG_OBSERVE_TX, &obs);
    nrf_get_reg(REG_CD, &cd);

    printf("config reg: %02x\n\
            status reg: %02x\n\
            fifo status reg: %02x\n\
            tx obs reg: %02x\n\
            carrier detec: %02x\n", config,
            status, fifo_status, obs, cd);
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

void fifo_put(uint8_t data)
{
    nrf_command_w(W_TX_PAYLOAD_NOACK, &data, 1);
}

int main(void)
{
    int c;

    CPU_PRESCALE(0); /* 16MHz */
    LED_INIT();
    usb_init();
    nrf_init();
    stdout = &mystdout;
   
    while(1) {
        printf(">>");
        do {
            c = usb_serial_getchar();
        } 
        while(!(c >= 0));
        printf("%c\n", c);
        switch (c) {
            case 's':
                print_regs();
                break;
            case 'f':
                nrf_command_w(FLUSH_TX, NULL, 0);
                break;
            case 't':
                nrf_tx_mode();
                break;
            case 'r':
                nrf_rx_mode();
                break;
            case 'p':
                fifo_put(c);            
                break;
            case 'a':
                nrf_fifo_single_tx();
                break;
            default:
                printf("pardon?\n");
                break;
        }

    }
    return 0;
}
