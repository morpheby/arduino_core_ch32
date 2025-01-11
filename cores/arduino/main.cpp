#define ARDUINO_MAIN

#include "Arduino.h"
#include "debug.h"

#if USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"

#ifndef ARDUINO_LOOP_STACK_SIZE
#define ARDUINO_LOOP_STACK_SIZE 512
#endif

#ifndef USBD_STACK_SZ
#define USBD_STACK_SZ 512
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
    xTaskCreate(usb_device_task, "usbd", USBD_STACK_SZ, NULL,
                configMAX_PRIORITIES - 1, NULL);
#endif
    setup();
    for (;;) {
#if USE_FREERTOS
        yieldIfNecessary();
#endif
        loop( );
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
static void loopTask(void *pvParameters) {
    loopFn();
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    taskDISABLE_INTERRUPTS();
    printf("Stack overflow in task: %s\n", pcTaskName);
    abort();
}

extern "C" void vApplicationMallocFailedHook(void) {
    taskDISABLE_INTERRUPTS();
    printf("Not enough memory");
    abort();
}

#endif

/*
 * \brief Main entry point of Arduino application
 */
int main( void )
{
    pre_init( );
    NVIC_PriorityGroupConfig(1);
    
#if USE_FREERTOS
    xTaskCreate(loopTask, "loopTask", ARDUINO_LOOP_STACK_SIZE, NULL, 1, &loopTaskHandle);
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
