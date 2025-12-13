
#if defined( FREERTOS_USE_ISP ) && ( FREERTOS_USE_ISP != 0 )
#define ISR
#elif defined( WCH_HPE_ENABLED ) && ( WCH_HPE_ENABLED != 0 )
#define ISR __attribute__((externally_visible, used, interrupt("WCH-interrupt-fast")))
#else
#define ISR __attribute__((externally_visible, used, interrupt()))
#endif

#define WEAK __attribute__((weak))

typedef void ISR (*VectorFunc)();

void ISR WEAK NMI_Handler() { while (1); }

void ISR WEAK AddressMisalignment_Handler() { while (1); }

void ISR WEAK FetchAccess_Handler() { while (1); }

void ISR WEAK ILL_Handler() { while (1); }

void ISR WEAK LoadMisalignment_Handler() { while (1); }

void ISR WEAK LoadAccess_Handler() { while (1); }

void ISR WEAK StoreMisalignment_Handler() { while (1); }

void ISR WEAK StoreAccess_Handler() { while (1); }

void ISR WEAK HardFault_Handler() { while (1); }

void ISR WEAK Ecall_M_Mode_Handler() { while (1); }

void ISR WEAK Ecall_U_Mode_Handler() { while (1); }

void ISR WEAK Break_Point_Handler() { while (1); }

void ISR WEAK SysTick_Handler() { while (1); }

void ISR WEAK SW_Handler() { while (1); }

void ISR WEAK WWDG_IRQHandler() { while (1); }

void ISR WEAK PVD_IRQHandler() { while (1); }

void ISR WEAK TAMPER_IRQHandler() { while (1); }

void ISR WEAK RTC_IRQHandler() { while (1); }

void ISR WEAK FLASH_IRQHandler() { while (1); }

void ISR WEAK RCC_IRQHandler() { while (1); }

void ISR WEAK EXTI0_IRQHandler() { while (1); }

void ISR WEAK EXTI1_IRQHandler() { while (1); }

void ISR WEAK EXTI2_IRQHandler() { while (1); }

void ISR WEAK EXTI3_IRQHandler() { while (1); }

void ISR WEAK EXTI4_IRQHandler() { while (1); }

void ISR WEAK DMA1_Channel1_IRQHandler() { while (1); }

void ISR WEAK DMA1_Channel2_IRQHandler() { while (1); }

void ISR WEAK DMA1_Channel3_IRQHandler() { while (1); }

void ISR WEAK DMA1_Channel4_IRQHandler() { while (1); }

void ISR WEAK DMA1_Channel5_IRQHandler() { while (1); }

void ISR WEAK DMA1_Channel6_IRQHandler() { while (1); }

void ISR WEAK DMA1_Channel7_IRQHandler() { while (1); }

void ISR WEAK ADC1_2_IRQHandler() { while (1); }

void ISR WEAK USB_HP_CAN1_TX_IRQHandler() { while (1); }

void ISR WEAK USB_LP_CAN1_RX0_IRQHandler() { while (1); }

void ISR WEAK CAN1_RX1_IRQHandler() { while (1); }

void ISR WEAK CAN1_SCE_IRQHandler() { while (1); }

void ISR WEAK EXTI9_5_IRQHandler() { while (1); }

void ISR WEAK TIM1_BRK_IRQHandler() { while (1); }

void ISR WEAK TIM1_UP_IRQHandler() { while (1); }

void ISR WEAK TIM1_TRG_COM_IRQHandler() { while (1); }

void ISR WEAK TIM1_CC_IRQHandler() { while (1); }

void ISR WEAK TIM2_IRQHandler() { while (1); }

void ISR WEAK TIM3_IRQHandler() { while (1); }

void ISR WEAK TIM4_IRQHandler() { while (1); }

void ISR WEAK I2C1_EV_IRQHandler() { while (1); }

void ISR WEAK I2C1_ER_IRQHandler() { while (1); }

void ISR WEAK I2C2_EV_IRQHandler() { while (1); }

void ISR WEAK I2C2_ER_IRQHandler() { while (1); }

void ISR WEAK SPI1_IRQHandler() { while (1); }

void ISR WEAK SPI2_IRQHandler() { while (1); }

void ISR WEAK USART1_IRQHandler() { while (1); }

void ISR WEAK USART2_IRQHandler() { while (1); }

void ISR WEAK USART3_IRQHandler() { while (1); }

void ISR WEAK EXTI15_10_IRQHandler() { while (1); }

void ISR WEAK RTCAlarm_IRQHandler() { while (1); }

void ISR WEAK USBWakeUp_IRQHandler() { while (1); }

void ISR WEAK USBHD_IRQHandler() { while (1); }

void ISR WEAK USBHDWakeUp_IRQHandler() { while (1); }

void ISR WEAK UART4_IRQHandler() { while (1); }

void ISR WEAK DMA1_Channel8_IRQHandler() { while (1); }

void ISR WEAK Undefined_IRQHandler() { while (1); }

extern void _start();

#if defined( FREERTOS_USE_ISP ) && ( FREERTOS_USE_ISP == 1 )

__attribute__((section(".vector"), externally_visible))
volatile const VectorFunc __MCU_Exceptions[] = {
    AddressMisalignment_Handler,
    FetchAccess_Handler,
    ILL_Handler,
    Break_Point_Handler,      
    LoadMisalignment_Handler,
    LoadAccess_Handler,      
    StoreMisalignment_Handler,
    StoreAccess_Handler,
    Ecall_U_Mode_Handler,   
    Undefined_IRQHandler,      
    Undefined_IRQHandler,
    Ecall_M_Mode_Handler,
};

__attribute__((section(".vector"), externally_visible))
volatile const VectorFunc __MCU_Vectors[] = {
    Undefined_IRQHandler,
    Undefined_IRQHandler,
    NMI_Handler,                /* NMI */
    Undefined_IRQHandler,      
    Undefined_IRQHandler,
    Undefined_IRQHandler,      
    Undefined_IRQHandler,
    Undefined_IRQHandler,
    Undefined_IRQHandler,   
    Undefined_IRQHandler,      
    Undefined_IRQHandler,
    Undefined_IRQHandler,
    SysTick_Handler,            /* SysTick */
    Undefined_IRQHandler,
    SW_Handler,                 /* SW */
    Undefined_IRQHandler,
    /* External Interrupts */
    WWDG_IRQHandler,            /* Window Watchdog */
    PVD_IRQHandler,             /* PVD through EXTI Line detect */
    TAMPER_IRQHandler,          /* TAMPER */
    RTC_IRQHandler,             /* RTC */
    FLASH_IRQHandler,           /* Flash */
    RCC_IRQHandler,             /* RCC */
    EXTI0_IRQHandler,           /* EXTI Line 0 */
    EXTI1_IRQHandler,           /* EXTI Line 1 */
    EXTI2_IRQHandler,           /* EXTI Line 2 */
    EXTI3_IRQHandler,           /* EXTI Line 3 */
    EXTI4_IRQHandler,           /* EXTI Line 4 */
    DMA1_Channel1_IRQHandler,   /* DMA1 Channel 1 */
    DMA1_Channel2_IRQHandler,   /* DMA1 Channel 2 */
    DMA1_Channel3_IRQHandler,   /* DMA1 Channel 3 */
    DMA1_Channel4_IRQHandler,   /* DMA1 Channel 4 */
    DMA1_Channel5_IRQHandler,   /* DMA1 Channel 5 */
    DMA1_Channel6_IRQHandler,   /* DMA1 Channel 6 */
    DMA1_Channel7_IRQHandler,   /* DMA1 Channel 7 */
    ADC1_2_IRQHandler,          /* ADC1_2 */
    USB_HP_CAN1_TX_IRQHandler,  /* USB HP and CAN1 TX */
    USB_LP_CAN1_RX0_IRQHandler, /* USB LP and CAN1RX0 */
    CAN1_RX1_IRQHandler,        /* CAN1 RX1 */
    CAN1_SCE_IRQHandler,        /* CAN1 SCE */
    EXTI9_5_IRQHandler,         /* EXTI Line 9..5 */
    TIM1_BRK_IRQHandler,        /* TIM1 Break */
    TIM1_UP_IRQHandler,         /* TIM1 Update */
    TIM1_TRG_COM_IRQHandler,    /* TIM1 Trigger and Commutation */
    TIM1_CC_IRQHandler,         /* TIM1 Capture Compare */
    TIM2_IRQHandler,            /* TIM2 */
    TIM3_IRQHandler,            /* TIM3 */
    TIM4_IRQHandler,            /* TIM4 */
    I2C1_EV_IRQHandler,         /* I2C1 Event */
    I2C1_ER_IRQHandler,         /* I2C1 Error */
    I2C2_EV_IRQHandler,         /* I2C2 Event */
    I2C2_ER_IRQHandler,         /* I2C2 Error */
    SPI1_IRQHandler,            /* SPI1 */
    SPI2_IRQHandler,            /* SPI2 */
    USART1_IRQHandler,          /* USART1 */
    USART2_IRQHandler,          /* USART2 */
    USART3_IRQHandler,          /* USART3 */
    EXTI15_10_IRQHandler,       /* EXTI Line 15..10 */
    RTCAlarm_IRQHandler,        /* RTC Alarm through EXTI Line */
    USBWakeUp_IRQHandler,       /* USB Wake up from suspend */
    USBHD_IRQHandler,           /* USBHD Break */
    USBHDWakeUp_IRQHandler,     /* USBHD Wake up from suspend */
    UART4_IRQHandler,           /* UART4 */
    DMA1_Channel8_IRQHandler,   /* DMA1 Channel8 */
    Undefined_IRQHandler,
};
#else
__attribute__((section(".vector"), externally_visible))
volatile const VectorFunc __MCU_Vectors[] = {
    _start,
    Undefined_IRQHandler,
    NMI_Handler,                /* NMI */
    HardFault_Handler,          /* Hard Fault */
    Undefined_IRQHandler,
    Ecall_M_Mode_Handler,       /* Ecall M Mode */
    Undefined_IRQHandler,
    Undefined_IRQHandler,
    Ecall_U_Mode_Handler,       /* Ecall U Mode */
    Break_Point_Handler,        /* Break Point */
    Undefined_IRQHandler,
    Undefined_IRQHandler,
    SysTick_Handler,            /* SysTick */
    Undefined_IRQHandler,
    SW_Handler,                 /* SW */
    Undefined_IRQHandler,
    /* External Interrupts */
    WWDG_IRQHandler,            /* Window Watchdog */
    PVD_IRQHandler,             /* PVD through EXTI Line detect */
    TAMPER_IRQHandler,          /* TAMPER */
    RTC_IRQHandler,             /* RTC */
    FLASH_IRQHandler,           /* Flash */
    RCC_IRQHandler,             /* RCC */
    EXTI0_IRQHandler,           /* EXTI Line 0 */
    EXTI1_IRQHandler,           /* EXTI Line 1 */
    EXTI2_IRQHandler,           /* EXTI Line 2 */
    EXTI3_IRQHandler,           /* EXTI Line 3 */
    EXTI4_IRQHandler,           /* EXTI Line 4 */
    DMA1_Channel1_IRQHandler,   /* DMA1 Channel 1 */
    DMA1_Channel2_IRQHandler,   /* DMA1 Channel 2 */
    DMA1_Channel3_IRQHandler,   /* DMA1 Channel 3 */
    DMA1_Channel4_IRQHandler,   /* DMA1 Channel 4 */
    DMA1_Channel5_IRQHandler,   /* DMA1 Channel 5 */
    DMA1_Channel6_IRQHandler,   /* DMA1 Channel 6 */
    DMA1_Channel7_IRQHandler,   /* DMA1 Channel 7 */
    ADC1_2_IRQHandler,          /* ADC1_2 */
    USB_HP_CAN1_TX_IRQHandler,  /* USB HP and CAN1 TX */
    USB_LP_CAN1_RX0_IRQHandler, /* USB LP and CAN1RX0 */
    CAN1_RX1_IRQHandler,        /* CAN1 RX1 */
    CAN1_SCE_IRQHandler,        /* CAN1 SCE */
    EXTI9_5_IRQHandler,         /* EXTI Line 9..5 */
    TIM1_BRK_IRQHandler,        /* TIM1 Break */
    TIM1_UP_IRQHandler,         /* TIM1 Update */
    TIM1_TRG_COM_IRQHandler,    /* TIM1 Trigger and Commutation */
    TIM1_CC_IRQHandler,         /* TIM1 Capture Compare */
    TIM2_IRQHandler,            /* TIM2 */
    TIM3_IRQHandler,            /* TIM3 */
    TIM4_IRQHandler,            /* TIM4 */
    I2C1_EV_IRQHandler,         /* I2C1 Event */
    I2C1_ER_IRQHandler,         /* I2C1 Error */
    I2C2_EV_IRQHandler,         /* I2C2 Event */
    I2C2_ER_IRQHandler,         /* I2C2 Error */
    SPI1_IRQHandler,            /* SPI1 */
    SPI2_IRQHandler,            /* SPI2 */
    USART1_IRQHandler,          /* USART1 */
    USART2_IRQHandler,          /* USART2 */
    USART3_IRQHandler,          /* USART3 */
    EXTI15_10_IRQHandler,       /* EXTI Line 15..10 */
    RTCAlarm_IRQHandler,        /* RTC Alarm through EXTI Line */
    USBWakeUp_IRQHandler,       /* USB Wake up from suspend */
    USBHD_IRQHandler,           /* USBHD Break */
    USBHDWakeUp_IRQHandler,     /* USBHD Wake up from suspend */
    UART4_IRQHandler,           /* UART4 */
    DMA1_Channel8_IRQHandler,   /* DMA1 Channel8 */
    Undefined_IRQHandler,
};
#endif
