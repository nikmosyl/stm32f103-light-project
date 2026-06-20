#include "stm32f1xx.h"

#if !defined(HSE_VALUE)
#define HSE_VALUE 8000000U
#endif

#if !defined(HSI_VALUE)
#define HSI_VALUE 8000000U
#endif

uint32_t SystemCoreClock = 72000000U;

const uint8_t AHBPrescTable[16U] = {0,0,0,0,0,0,0,0,1,2,3,4,6,7,8,9};
const uint8_t APBPrescTable[8U]  = {0,0,0,0,1,2,3,4};

static void SetSysClockTo72(void)
{
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0U) {}

    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2;

    RCC->CFGR = 0U;
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    RCC->CFGR |= RCC_CFGR_PLLSRC;
    RCC->CFGR |= RCC_CFGR_PLLXTPRE;
    RCC->CFGR &= ~RCC_CFGR_PLLMULL;
    RCC->CFGR |= RCC_CFGR_PLLMULL9;

    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0U) {}

    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}
}

void SystemInit(void)
{
    RCC->CR |= RCC_CR_HSION;
    RCC->CFGR = 0U;
    RCC->CR &= ~(RCC_CR_PLLON | RCC_CR_CSSON | RCC_CR_HSEON);
    RCC->CR &= ~RCC_CR_HSEBYP;
    RCC->CIR = 0U;

    SetSysClockTo72();

    SystemCoreClock = 72000000U;
}

void SystemCoreClockUpdate(void)
{
    uint32_t tmp = RCC->CFGR & RCC_CFGR_SWS;

    if (tmp == RCC_CFGR_SWS_HSI)
    {
        SystemCoreClock = HSI_VALUE;
    }
    else if (tmp == RCC_CFGR_SWS_HSE)
    {
        SystemCoreClock = HSE_VALUE;
    }
    else if (tmp == RCC_CFGR_SWS_PLL)
    {
        uint32_t pllsrc = RCC->CFGR & RCC_CFGR_PLLSRC;
        uint32_t pllmul = (RCC->CFGR & RCC_CFGR_PLLMULL) >> 18U;
        pllmul += 2U;

        if (pllsrc == 0U)
        {
            SystemCoreClock = (HSI_VALUE >> 1U) * pllmul;
        }
        else
        {
            if ((RCC->CFGR & RCC_CFGR_PLLXTPRE) != 0U)
            {
                SystemCoreClock = (HSE_VALUE >> 1U) * pllmul;
            }
            else
            {
                SystemCoreClock = HSE_VALUE * pllmul;
            }
        }
    }
    else
    {
        SystemCoreClock = HSI_VALUE;
    }

    uint32_t hpre = (RCC->CFGR & RCC_CFGR_HPRE) >> 4U;
    uint32_t shift = AHBPrescTable[hpre];
    SystemCoreClock >>= shift;
}
