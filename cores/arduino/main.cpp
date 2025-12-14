#define ARDUINO_MAIN

#include "Arduino.h"
#include "debug.h"

#if USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"

#ifndef ARDUINO_LOOP_STACK_SIZE
#define ARDUINO_LOOP_STACK_SIZE 1024
#endif

#ifndef USBD_STACK_SZ
#define USBD_STACK_SZ 256
#endif

void yieldIfNecessary(void) {
  static uint64_t lastYield = 0;
  uint64_t now = millis();
  if ((now - lastYield) > 2000) {
    lastYield = now;
    vTaskDelay(5);
  }
}

TaskHandle_t loopTaskHandle = NULL;

#if USE_TINYUSB

// USB Device Driver task
// This top level thread processes all usb events and invokes callbacks
static void usb_device_task(void *param) {
  (void)param;
    TinyUSB_Device_Init(0);
    // RTOS forever loop
    while (1) {
        TinyUSB_Device_Task();
    }
}
#endif

#endif


[[noreturn]] static void loopFn() {
#if USE_FREERTOS && USE_TINYUSB
    // Create a task for tinyusb device stack
    if (xTaskCreate(usb_device_task, "usbd", USBD_STACK_SZ, NULL,
                    configMAX_PRIORITIES - 1, NULL) != pdPASS) {
        puts("Couldn't start USB stack\n");
    }
#endif
    setup();
    for (;;) {
#if USE_FREERTOS
        yieldIfNecessary();
#endif
        loop( );
#if USE_BUFFERED_IO
        fflush(stdout);
        fflush(stderr);
#endif
#if USE_FREERTOS && USE_TINYUSB
        TinyUSB_Device_FlushCDC();
#endif
#if USE_TINYUSB && !(USE_FREERTOS)
        if (TinyUSB_Device_Task) {
            TinyUSB_Device_Task();
        }
        if (TinyUSB_Device_FlushCDC) {
            TinyUSB_Device_FlushCDC();
        }
#endif
    }
}

#if USE_FREERTOS

#if configSUPPORT_STATIC_ALLOCATION
static StackType_t loopTaskStack[ARDUINO_LOOP_STACK_SIZE];
static StaticTask_t loopTaskTCB;
#endif

static void loopTask(void *pvParameters) {
    loopFn();
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    taskDISABLE_INTERRUPTS();
    puts("Stack overflow in task ");
    puts(pcTaskName);
    while(1) { }
}

extern "C" void vApplicationMallocFailedHook(void) {
    if (xPortIsInsideInterrupt()) {
        while(1) { }
    } else {
        puts("Not enough memory");
        while(1) { }
    }
}

/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
   implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
   used by the Idle task. */
extern "C" void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
    /* If the buffers to be provided to the Idle task are declared inside this
       function then they must be declared static - otherwise they will be allocated on
       the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
       state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
       Note that, as the array is necessarily of type StackType_t,
       configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/*-----------------------------------------------------------*/

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
   application must provide an implementation of vApplicationGetTimerTaskMemory()
   to provide the memory that is used by the Timer service task. */
extern "C" void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
    /* If the buffers to be provided to the Timer task are declared inside this
       function then they must be declared static - otherwise they will be allocated on
       the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
       task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
       Note that, as the array is necessarily of type StackType_t,
      configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

#endif

/*
 * \brief Main entry point of Arduino application
 */
int __attribute__((used)) main( void )
{
    pre_init( );
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

#if !USE_BUFFERED_IO
    // Disable buffering on stdout/stderr
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
#else if USE_BUFFERED_IO > 1
    // Enable buffering on stdout/stderr
    static char stdout_buf[USE_BUFFERED_IO];
    static char stderr_buf[USE_BUFFERED_IO];
    setbuf(stdout, stdout_buf);
    setbuf(stderr, stderr_buf);
#endif
    
#if USE_FREERTOS
#if configSUPPORT_STATIC_ALLOCATION
    loopTaskHandle = xTaskCreateStatic(loopTask, "loopTask", ARDUINO_LOOP_STACK_SIZE, NULL, 1, loopTaskStack, &loopTaskTCB);
#else
    xTaskCreate(loopTask, "loopTask", ARDUINO_LOOP_STACK_SIZE, NULL, 1, &loopTaskHandle);
#endif
    vTaskStartScheduler();
#else
#if USE_TINYUSB
    if (TinyUSB_Device_Init) {
        TinyUSB_Device_Init(0);
    }
#endif
    loopFn();
#endif

    return 0;
}
