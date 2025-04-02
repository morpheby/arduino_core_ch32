
typedef void __attribute__((interrupt, used)) (*VectorFunc)();

void __attribute__((interrupt, externally_visible, weak, used)) NMI_Handler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) HardFault_Handler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) Ecall_M_Mode_Handler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) Ecall_U_Mode_Handler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) Break_Point_Handler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) SysTick_Handler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) SW_Handler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) WWDG_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) PVD_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) TAMPER_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) RTC_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) FLASH_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) RCC_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) EXTI0_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) EXTI1_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) EXTI2_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) EXTI3_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) EXTI4_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) DMA1_Channel1_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) DMA1_Channel2_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) DMA1_Channel3_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) DMA1_Channel4_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) DMA1_Channel5_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) DMA1_Channel6_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) DMA1_Channel7_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) ADC1_2_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) USB_HP_CAN1_TX_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) USB_LP_CAN1_RX0_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) CAN1_RX1_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) CAN1_SCE_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) EXTI9_5_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) TIM1_BRK_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) TIM1_UP_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) TIM1_TRG_COM_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) TIM1_CC_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) TIM2_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) TIM3_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) TIM4_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) I2C1_EV_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) I2C1_ER_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) I2C2_EV_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) I2C2_ER_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) SPI1_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) SPI2_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) USART1_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) USART2_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) USART3_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) EXTI15_10_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) RTCAlarm_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) USBWakeUp_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) USBHD_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) USBHDWakeUp_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) UART4_IRQHandler() { while (1); }

void __attribute__((interrupt, externally_visible, weak, used)) DMA1_Channel8_IRQHandler() { while (1); }

void _start();

__attribute__((section(".vector"), externally_visible))
volatile const VectorFunc __MCU_Vectors[] = {
    _start,
    0,
    NMI_Handler,                /* NMI */
    HardFault_Handler,          /* Hard Fault */
    0,
    Ecall_M_Mode_Handler,       /* Ecall M Mode */
    0,
    0,
    Ecall_U_Mode_Handler,       /* Ecall U Mode */
    Break_Point_Handler,        /* Break Point */
    0,
    0,
    SysTick_Handler,            /* SysTick */
    0,
    SW_Handler,                 /* SW */
    0,
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
};
