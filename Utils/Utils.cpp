#include "stm32f4xx.h"
#include "Utils.hpp"

void Utils::init()
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    DWT->CYCCNT = 0;

    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void Utils::delayUs(uint16_t us)
{
	uint32_t start = DWT->CYCCNT;

	uint32_t cycles = us * (SystemCoreClock / 1'000'000);

    while ((DWT->CYCCNT - start) < cycles)
    {
    }
}
