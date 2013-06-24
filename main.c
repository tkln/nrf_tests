#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

int main(void)
{
    CPU_PRESCALE(0); /* 16MHz */
    return 0;
}
