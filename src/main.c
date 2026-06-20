#include "stm32f1xx.h"

#define LED_PIN     13U
#define LED_MASK    (1U << LED_PIN)

static void delay(volatile uint32_t ticks)
{
    while (ticks-- != 0U)
    {
        __NOP();
    }
}

int main(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_1;

    GPIOC->BSRR = LED_MASK;

    while (1)
    {
        GPIOC->ODR ^= LED_MASK;
        delay(800000U);
    }
}
