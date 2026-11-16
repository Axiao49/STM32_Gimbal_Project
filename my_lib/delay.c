#include "main.h"



//
// @简介：微秒级延迟
// @参数：us - 要延迟的时间，单位是微秒
//
void DelayUs(uint32_t us)
{
	
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    
    // 获取 SysTick 的自动重装载值
    uint32_t reload = SysTick->LOAD; 
    
    // 计算需要的 tick 数
    ticks = us * (SystemCoreClock / 1000000); 
    
    tcnt = 0;
    told = SysTick->VAL; 
    
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += reload - tnow + told;
            told = tnow;
            
            if (tcnt >= ticks) 
                break; 
        }
    }
}


