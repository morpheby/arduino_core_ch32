
#include "ch32_core_interrupts.h"
#include "ch32v30x_isr.h"
#include <ch32vxxx/ch32vxxx_isr.h>
#include <stdint.h>
#include <assert.h>

#ifndef NDEBUG 
    #define DBGREG volatile
#else
    #define DBGREG
#endif

#if CH32_GLOBAL_ISR
_ISR_DEF(freertos_risc_v_application_exception_handler) {
    DBGREG uint32_t mcause;
    
    __asm__ volatile ("csrr %0, mcause" : "=r"(mcause));
    
    #ifndef NDEBUG
    DBGREG uint32_t mepc, mstatus;
    __asm__ volatile ("csrr %0, mepc" : "=r"(mepc));
    __asm__ volatile ("csrr %0, mstatus" : "=r"(mstatus));
    #endif
    
    assert((mcause & 0x7FFFFFFFu) < CH32_EXCEPTION_TABLE_SIZE);
    __MCU_Exceptions[mcause & 0x7FFFFFFFu]();
}

_ISR_DEF(freertos_risc_v_application_interrupt_handler) {
    DBGREG uint32_t mcause;
    
    __asm__ volatile ("csrr %0, mcause" : "=r"(mcause));
    
    #ifndef NDEBUG
    DBGREG uint32_t mepc, mstatus;
    __asm__ volatile ("csrr %0, mepc" : "=r"(mepc));
    __asm__ volatile ("csrr %0, mstatus" : "=r"(mstatus));
    #endif
    
    assert((mcause & 0x7FFFFFFFu) < CH32_IRQ_TABLE_SIZE);
    __MCU_Interrupts[mcause & 0x7FFFFFFFu]();
}
#endif
