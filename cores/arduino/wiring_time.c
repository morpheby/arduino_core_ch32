/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Arduino.h"

#if USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

uint32_t millis(void)
{
  return getCurrentMillis();
}

// Interrupt-compatible version of micros
uint64_t micros(void)
{
  return getCurrentMicros();
}

void delay(uint32_t ms)
{
  
#if USE_FREERTOS
  vTaskDelay(pdMS_TO_TICKS(ms));
#else
  if (ms != 0) {
    uint32_t start = getCurrentMillis();
    do {
      yield();
    } while (getCurrentMillis() - start < ms);
  }
#endif
}

#ifndef CH32V10x
void delayMicroseconds(uint32_t us)
{
  uint64_t currentTicks = SysTick->CNT;
  /* Number of ticks to count */
  #if USE_FREERTOS
  const uint64_t tickPerMs = ((uint64_t)configCPU_CLOCK_HZ)/1000ULL;
  const uint64_t nbTicks = ((((uint64_t)us) - ((us > 0) ? 1 : 0)) * tickPerMs) / 1000ULL;
  const uint64_t targetTicks = nbTicks + currentTicks;
  do {
    currentTicks = SysTick->CNT;
  } while (targetTicks > currentTicks);
  #else
  /* Number of ticks per millisecond */
  const uint64_t tickPerMs = SysTick->CMP + 1;
  const uint64_t nbTicks = ((us - ((us > 0) ? 1 : 0)) * tickPerMs) / 1000;
  /* Number of elapsed ticks */
  uint64_t elapsedTicks = 0;
  uint64_t oldTicks = currentTicks;
  do {
    currentTicks = SysTick->CNT;
    // elapsedTicks += (oldTicks < currentTicks) ? tickPerMs + oldTicks - currentTicks :
    //                 oldTicks - currentTicks;
    
    //increment
    elapsedTicks += (oldTicks <= currentTicks) ? currentTicks - oldTicks :
                     tickPerMs - oldTicks + currentTicks;

    oldTicks = currentTicks;
  } while (nbTicks > elapsedTicks);  
  #endif
}
#else
#define SYSTICK_CNTL    (0xE000F004)   
#define SYSTICK_CNTH    (0xE000F008)
#define SYSTICK_CMPL    (0xE000F00C)
#define SYSTICK_CMPH    (0xE000F010)

static inline void delayMicroseconds(uint32_t) __attribute__((always_inline, unused));
static inline void delayMicroseconds(uint32_t us)
{
  __IO uint64_t currentTicks = *((__IO uint32_t *)SYSTICK_CNTH);
                currentTicks = (currentTicks << 32) +  *((__IO uint32_t *)SYSTICK_CNTL); 
  /* Number of ticks per millisecond */
  uint64_t tickPerMs = *((__IO uint32_t *)SYSTICK_CMPH);
           tickPerMs = (tickPerMs << 32) + *((__IO uint32_t *)SYSTICK_CMPL) + 1;        
  /* Number of ticks to count */
  uint64_t nbTicks = ((us - ((us > 0) ? 1 : 0)) * tickPerMs) / 1000;
  /* Number of elapsed ticks */
  uint64_t elapsedTicks = 0;
  __IO uint64_t oldTicks = currentTicks;
  do {
    currentTicks = *((__IO uint32_t *)SYSTICK_CNTH);
    currentTicks = (currentTicks << 32) +  *((__IO uint32_t *)SYSTICK_CNTL); 
    //increment
    elapsedTicks += (oldTicks <= currentTicks) ? currentTicks - oldTicks :
                     tickPerMs - oldTicks + currentTicks;

    oldTicks = currentTicks;
  } while (nbTicks > elapsedTicks);  
}
#endif



#ifdef __cplusplus
}
#endif
