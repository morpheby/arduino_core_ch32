;/********************************** (C) COPYRIGHT *******************************
;* File Name          : startup_ch32v20x_D6.s
;* Author             : WCH
;* Version            : V1.0.0
;* Date               : 2021/06/06
;* Description        :  CH32V203F6-CH32V203F8-CH32V203G6-CH32V203G8-CH32V203K6-CH32V203K8-CH32V203C6-CH32V203C8
;*                    vector table for eclipse toolchain.
;* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
;* SPDX-License-Identifier: Apache-2.0
;*******************************************************************************/

#if defined( FREERTOS_USE_ISP ) && ( FREERTOS_USE_ISP != 0 )
	.extern Global_IRQ_Handler
#endif
	
	.section	.init,"ax",@progbits
	.global	_start
	.align	1
_start:
	 
	j	handle_reset
	.word 0x00000013
	.word 0x00000013
	.word 0x00000013
	.word 0x00000013
	.word 0x00000013
	.word 0x00000013
	.word 0x00000013
	.word 0x00000013
	.word 0x00000013
	.word 0x00000013
	.word 0x00000013
	.word 0x00000013
	.word 0x00100073
    
    .option rvc;

	.section	.text.handle_reset,"ax",@progbits
	.weak	handle_reset
	.align	1
handle_reset:
.option push 
.option	norelax 
	la gp, __global_pointer$
.option	pop 
1:
	la sp, _eusrstack 
2:
	/* Load data section from flash to RAM */
	la a0, _data_lma
	la a1, _data_vma
	la a2, _edata
	bgeu a1, a2, 2f
1:
	lw t0, (a0)
	sw t0, (a1)
	addi a0, a0, 4
	addi a1, a1, 4
	bltu a1, a2, 1b
2:
	/* Clear bss section */
	la a0, _sbss
	la a1, _ebss
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
2:
    li t0, 0x1f
    csrw 0xbc0, t0

    #if defined( WCH_HPE_ENABLED ) && ( WCH_HPE_ENABLED != 0 )

    /* Enable nested and hardware stack */
	li t0, 0x3
	csrw 0x804, t0

	#else

	/* Enable nested  */
    /* li t0, 0x2 */
	li t0, 0x0
    csrw 0x804, t0

	#endif

    #if defined(__PIO_BUILD_FREERTOS__) || defined(__PIO_BUILD_HARMONY_LITEOS__) || defined(__PIO_BUILD_RT_THREAD__) || defined(__PIO_BUILD_TENCENT_OS__)

    /* Enable interrupt */
    li t0, 0x1800
    csrw mstatus, t0

    #else
	
    /* Enable interrupt */
   	li t0, 0x88          
    csrw mstatus, t0
	
    #endif

#if defined( FREERTOS_USE_ISP ) && ( FREERTOS_USE_ISP != 0 )
	la t0, Global_IRQ_Handler
#else
	// Use vector table
 	la t0, __MCU_Vectors
    ori t0, t0, 3
#endif
	csrw mtvec, t0

    la a0, __libc_fini_array
    call atexit
    call __libc_init_array

    jal  SystemInit
	la t0, main
	csrw mepc, t0
	mret
