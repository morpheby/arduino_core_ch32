
#include "ch32_core_interrupts.h"
#include <ch32vxxx/ch32vxxx_isr.h>
#include <stdint.h>
#include <assert.h>

_ISR_DEF(freertos_risc_v_application_exception_handler) {
    uint32_t mcause, mepc, mstatus;
    __asm__ volatile ("csrr %0, mcause" : "=r"(mcause));
    __asm__ volatile ("csrr %0, mepc" : "=r"(mepc));
    __asm__ volatile ("csrr %0, mstatus" : "=r"(mstatus));
    
    assert((mcause & 0x7FFFFFFF) < CH32_EXCEPTION_TABLE_SIZE);
    __MCU_Exceptions[mcause & 0x7FFFFFFF]();
}

_ISR_DEF(freertos_risc_v_application_interrupt_handler) {
    uint32_t mcause, mepc, mstatus;
    __asm__ volatile ("csrr %0, mcause" : "=r"(mcause));
    __asm__ volatile ("csrr %0, mepc" : "=r"(mepc));
    __asm__ volatile ("csrr %0, mstatus" : "=r"(mstatus));
    
    assert((mcause & 0x7FFFFFFF) < CH32_IRQ_TABLE_SIZE);
    __MCU_Interrupts[mcause & 0x7FFFFFFF]();
}
