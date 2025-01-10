#define ARDUINO_MAIN

#include "Arduino.h"
#include "debug.h"

#if USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"

#ifndef ARDUINO_LOOP_STACK_SIZE
#define ARDUINO_LOOP_STACK_SIZE 8192
#endif

void yieldIfNecessary(void) {
  static uint64_t lastYield = 0;
  uint64_t now = millis();
  if ((now - lastYield) > 2000) {
    lastYield = now;
    vTaskDelay(5);  //delay 1 RTOS tick
  }
}

TaskHandle_t loopTaskHandle = NULL;

#endif


[[noreturn]] static void loopFn() {
#if defined(USE_TINYUSB)
    if (TinyUSB_Device_Init) {
        TinyUSB_Device_Init(0);
    }
#endif
    setup();
    for (;;) {
#if USE_FREERTOS
        yieldIfNecessary();
#endif
        loop( );
#if defined(USE_TINYUSB)
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
void loopTask(void *pvParameters) {
    loopFn();
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
    loopFn();
#endif

    return 0;
}
