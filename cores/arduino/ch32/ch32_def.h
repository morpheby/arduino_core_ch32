#ifndef _CH32_DEF_
#define _CH32_DEF_

#include "ch32vxxx.h"

#ifdef CH32_MCU_FAMILY_CH32V20x
  #define CH32V20x
#elif CH32_MCU_FAMILY_CH32V205
  #define CH32V205
#elif CH32_MCU_FAMILY_CH32V30x
  #define CH32V30x
#else
  #error "Unsupported CH32_MCU_FAMILY: " CH32_MCU_FAMILY
#endif

#ifndef F_CPU
  #define F_CPU SystemCoreClock
#endif

// Here define some compatibility
#ifndef ADC1
  #define ADC1 ADC
#endif
#ifndef CAN1
  #define CAN1 CAN
#endif
#ifndef DAC1           
  #define DAC1 DAC   //two independent channels
#endif


/**
 * Libc porting layers
 */
#if defined (  __GNUC__  ) /* GCC CS3 */
  #define WEAK __attribute__ ((weak))
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#if defined(NDEBUG)
#if !defined(_Error_Handler)
#define _Error_Handler(str, value) \
  printf("Arduino exception: %s, %i\n", str, (int) value); \
  while (1) {\
  }
#endif
#if !defined(Error_Handler)
#define Error_Handler() \
  printf("Arduino exception"); \
  while (1) {\
  }
#endif
#else
void _Error_Handler(const char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#endif

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif //_CH32_DEF_
