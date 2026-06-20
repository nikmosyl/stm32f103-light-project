#include "stm32f1xx.h"

#if !defined(HSE_VALUE)
#define HSE_VALUE ((uint32_t)8000000)
#endif

#if !defined(HSI_VALUE)
#define HSI_VALUE ((uint32_t)8000000)
#endif

#define VECT_TAB_OFFSET 0x0

uint32_t SystemCoreClock = HSI_VALUE;

const uint8_t AHBPrescTable[16] =
{
    0,0,0,0,0,0,0,0,1,2,3,4,6,7,8,9
};

const uint8_t APBPrescTable[8] =
{
    0,0,0,0,1,2,3,4
};

void SystemInit(void)
{
    RCC->CR |= 0x00000001;

#if !defined(STM32F105xC) && !defined(STM32F107xC)
    RCC->CFGR &= 0xF8FF0000;
#else
    RCC->CFGR &= 0xF0FF0000;
#endif

    RCC->CR &= 0xFEF6FFFF;
    RCC->CR &= 0xFFFBFFFF;
    RCC->CFGR &= 0xFF80FFFF;

#if defined(STM32F105xC) || defined(STM32F107xC)
    RCC->CR &= 0xEBFFFFFF;
    RCC->CIR = 0x00FF0000;
    RCC->CFGR2 = 0x00000000;
#elif defined(STM32F100xB) || defined(STM32F100xE)
    RCC->CIR = 0x009F0000;
    RCC->CFGR2 = 0x00000000;
#else
    RCC->CIR = 0x009F0000;
#endif

    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;
}

void SystemCoreClockUpdate(void)
{
    uint32_t tmp = 0;
    uint32_t pllmull = 0;
    uint32_t pllsource = 0;

    tmp = RCC->CFGR & RCC_CFGR_SWS;

    switch (tmp)
    {
        case 0x00:
            SystemCoreClock = HSI_VALUE;
            break;

        case 0x04:
            SystemCoreClock = HSE_VALUE;
            break;

        case 0x08:
            pllmull = (RCC->CFGR & RCC_CFGR_PLLMULL) >> 18;
            pllmull += 2;

            pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;

            if (pllsource == 0)
            {
                SystemCoreClock = (HSI_VALUE >> 1) * pllmull;
            }
            else
            {
                if (RCC->CFGR & RCC_CFGR_PLLXTPRE)
                    SystemCoreClock = (HSE_VALUE >> 1) * pllmull;
                else
                    SystemCoreClock = HSE_VALUE * pllmull;
            }
            break;

        default:
            SystemCoreClock = HSI_VALUE;
            break;
    }

    tmp = AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE) >> 4];
    SystemCoreClock >>= tmp;
}
