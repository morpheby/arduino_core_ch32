/*
 *******************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * All rights reserved.
 *
 * This software component is licensed by WCH under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
#include "analog.h"
#include "ch32yyxx_adc.h"
#include "PinAF_ch32yyxx.h"

#if USE_FREERTOS

#include "FreeRTOS.h"
#include "task.h"

#endif

#ifdef __cplusplus
extern "C" {
#endif

#if USE_FREERTOS
static volatile bool conversionDone = false;
static volatile TaskHandle_t adcWaitingTaskHandle;

__attribute__((externally_visible, interrupt))
void ADC1_2_IRQHandler(void) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  if (adcWaitingTaskHandle) {
    vTaskNotifyGiveFromISR(adcWaitingTaskHandle, &xHigherPriorityTaskWoken);
  }

  if (xHigherPriorityTaskWoken != pdFALSE) { taskYIELD (); }

  ADC_ClearITPendingBit(ADC1, ADC_IT_EOC | ADC_IT_AWD | ADC_IT_JEOC);
  ADC_ClearITPendingBit(ADC2, ADC_IT_EOC | ADC_IT_AWD | ADC_IT_JEOC);
  conversionDone = true;
}

#endif

/* Private_Variables */
#if (defined(ADC_MODULE_ENABLED) && !defined(ADC_MODULE_ONLY)) ||\
    (defined(DAC_MODULE_ENABLED) && !defined(DAC_MODULE_ONLY))
static PinName g_current_pin = NC;
static uint16_t calibration_value_adc1 = 0;
static uint16_t calibration_value_adc2 = 0;
#endif

/* Private_Defines */
#if defined(ADC_MODULE_ENABLED) && !defined(ADC_MODULE_ONLY)

#if (defined(CH32V20x) || defined(CH32V30x) || defined(CH32V30x_C) || defined(CH32V10x) || defined(CH32L10x) || defined(CH32VM00X) )

/* Default to use maximum sampling period */
#if defined(CH32L10x) || defined(CH32VM00X)

#ifndef ADC_SAMPLINGTIME
#if defined(ADC_SampleTime_CyclesMode7) 
#define ADC_SAMPLINGTIME        ADC_SampleTime_CyclesMode7
#elif defined(ADC_SampleTime_CyclesMode6)
#define ADC_SAMPLINGTIME        ADC_SampleTime_CyclesMode6
#elif defined(ADC_SampleTime_CyclesMode5)
#define ADC_SAMPLINGTIME        ADC_SampleTime_CyclesMode5
#elif defined(ADC_SampleTime_CyclesMode4)
#define ADC_SAMPLINGTIME        ADC_SampleTime_CyclesMode4
#elif defined(ADC_SampleTime_CyclesMode3)
#define ADC_SAMPLINGTIME        ADC_SampleTime_CyclesMode3
#elif defined(ADC_SampleTime_CyclesMode2)
#define ADC_SAMPLINGTIME        ADC_SampleTime_CyclesMode2
#elif defined(ADC_SampleTime_CyclesMode1)
#define ADC_SAMPLINGTIME        ADC_SampleTime_CyclesMode1
#elif defined(ADC_SampleTime_CyclesMode0)
#define ADC_SAMPLINGTIME        ADC_SampleTime_CyclesMode0
#endif
#endif /* !ADC_SAMPLINGTIME */

#else

#ifndef ADC_SAMPLINGTIME
#if defined(ADC_SampleTime_239Cycles5) 
#define ADC_SAMPLINGTIME        ADC_SampleTime_239Cycles5
#elif defined(ADC_SampleTime_71Cycles5)
#define ADC_SAMPLINGTIME        ADC_SampleTime_71Cycles5
#elif defined(ADC_SampleTime_55Cycles5)
#define ADC_SAMPLINGTIME        ADC_SampleTime_55Cycles5
#elif defined(ADC_SampleTime_41Cycles5)
#define ADC_SAMPLINGTIME        ADC_SampleTime_41Cycles5
#elif defined(ADC_SampleTime_28Cycles5)
#define ADC_SAMPLINGTIME        ADC_SampleTime_28Cycles5
#elif defined(ADC_SampleTime_13Cycles5)
#define ADC_SAMPLINGTIME        ADC_SampleTime_13Cycles5
#elif defined(ADC_SampleTime_7Cycles5)
#define ADC_SAMPLINGTIME        ADC_SampleTime_7Cycles5
#elif defined(ADC_SampleTime_1Cycles5)
#define ADC_SAMPLINGTIME        ADC_SampleTime_1Cycles5
#endif
#endif /* !ADC_SAMPLINGTIME */

#endif /* CH32L10x */


/*
 * Default to use maximum sampling period 
 */

#if defined(CH32L10x) || defined(CH32VM00X)

#ifndef ADC_SAMPLINGTIME_INTERNAL
#if defined(ADC_SampleTime_CyclesMode7)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_CyclesMode7
#elif defined(ADC_SampleTime_CyclesMode6)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_CyclesMode6
#elif defined(ADC_SampleTime_CyclesMode5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_CyclesMode5
#elif defined(ADC_SampleTime_CyclesMode4)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_CyclesMode4
#elif defined(ADC_SampleTime_CyclesMode3)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_CyclesMode3
#elif defined(ADC_SampleTime_CyclesMode2)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_CyclesMode2
#elif defined(ADC_SampleTime_CyclesMode1)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_CyclesMode1
#elif defined(ADC_SampleTime_CyclesMode0)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_CyclesMode0
#else
#error "ADC sampling time could not be defined for internal channels!"
#endif
#endif /* !ADC_SAMPLINGTIME_INTERNAL */

#else

#ifndef ADC_SAMPLINGTIME_INTERNAL
#if defined(ADC_SampleTime_239Cycles5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_239Cycles5
#elif defined(ADC_SampleTime_71Cycles5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_71Cycles5
#elif defined(ADC_SampleTime_55Cycles5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_55Cycles5
#elif defined(ADC_SampleTime_41Cycles5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_41Cycles5
#elif defined(ADC_SampleTime_28Cycles5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_28Cycles5
#elif defined(ADC_SampleTime_13Cycles5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_13Cycles5
#elif defined(ADC_SampleTime_7Cycles5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_7Cycles5
#elif defined(ADC_SampleTime_1Cycles5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_1Cycles5
#else
#error "ADC sampling time could not be defined for internal channels!"
#endif
#endif /* !ADC_SAMPLINGTIME_INTERNAL */


#endif



#ifndef ADC_CLOCK_DIV
#ifdef RCC_PCLK2_Div8
#define ADC_CLOCK_DIV       RCC_PCLK2_Div8
#elif defined(RCC_PCLK2_Div6) 
#define ADC_CLOCK_DIV       RCC_PCLK2_Div6
#elif defined(RCC_PCLK2_Div4)
#define ADC_CLOCK_DIV       RCC_PCLK2_Div4
#elif defined(RCC_PCLK2_Div2)
#define ADC_CLOCK_DIV       RCC_PCLK2_Div2
#endif
#endif /* !ADC_CLOCK_DIV */

#elif defined(CH32V00x)

/* Default to use maximum sampling period */
#ifndef ADC_SAMPLINGTIME
#if defined(ADC_SampleTime_241Cycles)
#define ADC_SAMPLINGTIME        ADC_SampleTime_241Cycles;
#elif defined(ADC_SampleTime_73Cycles)
#define ADC_SAMPLINGTIME        ADC_SampleTime_73Cycles;
#elif defined(ADC_SampleTime_57Cycles)
#define ADC_SAMPLINGTIME        ADC_SampleTime_57Cycles;
#elif defined(ADC_SampleTime_43Cycles)
#define ADC_SAMPLINGTIME        ADC_SampleTime_43Cycles;
#elif defined(ADC_SampleTime_30Cycles)
#define ADC_SAMPLINGTIME        ADC_SampleTime_30Cycles;
#elif defined(ADC_SampleTime_15Cycles)
#define ADC_SAMPLINGTIME        ADC_SampleTime_15Cycles;
#elif defined(ADC_SampleTime_9Cycles)
#define ADC_SAMPLINGTIME        ADC_SampleTime_9Cycles;
#elif defined(ADC_SampleTime_3Cycles)
#define ADC_SAMPLINGTIME        ADC_SampleTime_3Cycles
#endif
#endif /* !ADC_SAMPLINGTIME */

/*
 * Default to use maximum sampling period 
 */
#ifndef ADC_SAMPLINGTIME_INTERNAL
#if defined(ADC_SampleTime_241Cycles)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_241Cycles
#elif defined(ADC_SampleTime_73Cycles)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_73Cycles
#elif defined(ADC_SampleTime_57Cycles)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_57Cycles
#elif defined(ADC_SampleTime_43Cycles)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_43Cycles
#elif defined(ADC_SampleTime_30Cycles)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_30Cycles
#elif defined(ADC_SampleTime_15Cycles)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_15Cycles
#elif defined(ADC_SampleTime_9Cycles)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_9Cycles
#elif defined(ADC_SampleTime_3Cycles)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_3Cycles
#else
#error "ADC sampling time could not be defined for internal channels!"
#endif
#endif /* !ADC_SAMPLINGTIME_INTERNAL */

#ifndef ADC_CLOCK_DIV
#ifdef RCC_PCLK2_Div8
#define ADC_CLOCK_DIV       RCC_PCLK2_Div8
#elif defined(RCC_PCLK2_Div6) 
#define ADC_CLOCK_DIV       RCC_PCLK2_Div6
#elif defined(RCC_PCLK2_Div4)
#define ADC_CLOCK_DIV       RCC_PCLK2_Div4
#elif defined(RCC_PCLK2_Div2)
#define ADC_CLOCK_DIV       RCC_PCLK2_Div2
#endif
#endif /* !ADC_CLOCK_DIV */


#elif defined(CH32X035)



/* Default to use maximum sampling period */
#ifndef ADC_SAMPLINGTIME
#if defined(ADC_SampleTime_11Cycles)
#define ADC_SAMPLINGTIME        ADC_SampleTime_11Cycles;
#elif defined(ADC_SampleTime_10Cycles)
#define ADC_SAMPLINGTIME        ADC_SampleTime_10Cycles;
#elif defined(ADC_SampleTime_9Cycles)
#define ADC_SAMPLINGTIME        ADC_SampleTime_9Cycles;
#elif defined(ADC_SampleTime_8Cycles)
#define ADC_SAMPLINGTIME        ADC_SampleTime_8Cycles;
#elif defined(ADC_SampleTime_7Cycles)
#define ADC_SAMPLINGTIME        ADC_SampleTime_7Cycles;
#elif defined(ADC_SampleTime_6Cycles)
#define ADC_SAMPLINGTIME        ADC_SampleTime_6Cycles;
#elif defined(ADC_SampleTime_5Cycles)
#define ADC_SAMPLINGTIME        ADC_SampleTime_5Cycles;
#elif defined(ADC_SampleTime_4Cycles)
#define ADC_SAMPLINGTIME        ADC_SampleTime_4Cycles
#endif
#endif /* !ADC_SAMPLINGTIME */

/*
 * Default to use maximum sampling period 
 */
#ifndef ADC_SAMPLINGTIME_INTERNAL
#if defined(ADC_SampleTime_11Cycles)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_11Cycles
#elif defined(ADC_SampleTime_10Cycles)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_10Cycles
#elif defined(ADC_SampleTime_9Cycles)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_9Cycles
#elif defined(ADC_SampleTime_8Cycles)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_8Cycles
#elif defined(ADC_SampleTime_7Cycles)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_7Cycles
#elif defined(ADC_SampleTime_6Cycles)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_6Cycles
#elif defined(ADC_SampleTime_5Cycles)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_5Cycles
#elif defined(ADC_SampleTime_4Cycles)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SampleTime_4Cycles
#else
#error "ADC sampling time could not be defined for internal channels!"
#endif
#endif /* !ADC_SAMPLINGTIME_INTERNAL */

#ifndef ADC_CLOCK_DIV
#ifdef ADC_CLK_Div8
#define ADC_CLOCK_DIV       ADC_CLK_Div8
#elif defined(ADC_CLK_Div6) 
#define ADC_CLOCK_DIV       ADC_CLK_Div6
#elif defined(ADC_CLK_Div5)
#define ADC_CLOCK_DIV       ADC_CLK_Div5
#elif defined(ADC_CLK_Div4)
#define ADC_CLOCK_DIV       ADC_CLK_Div4
#endif
#endif /* !ADC_CLOCK_DIV */




#endif /* !CH32V00x */


#ifndef ADC_REGULAR_RANK_1
#define ADC_REGULAR_RANK_1  1
#endif

/* Exported Functions */
/**
  * @brief  Return ADC HAL channel linked to a PinName
  * @param  pin: PinName
  * @retval Valid HAL channel
  */
uint32_t get_adc_channel(PinName pin)
{
  uint32_t function = pinmap_function(pin, PinMap_ADC);
  uint32_t channel = 0;
  switch (CH_PIN_CHANNEL(function)) {
#ifdef ADC_Channel_0
    case 0:
      channel = ADC_Channel_0;
      break;
#endif
    case 1:
      channel = ADC_Channel_1;
      break;
    case 2:
      channel = ADC_Channel_2;
      break;
    case 3:
      channel = ADC_Channel_3;
      break;
    case 4:
      channel = ADC_Channel_4;
      break;
    case 5:
      channel = ADC_Channel_5;
      break;
    case 6:
      channel = ADC_Channel_6;
      break;
    case 7:
      channel = ADC_Channel_7;
      break;
    case 8:
      channel = ADC_Channel_8;
      break;
#ifdef ADC_Channel_9  
    case 9:
      channel = ADC_Channel_9;
      break;
#endif
#ifdef ADC_Channel_10      
    case 10:
      channel = ADC_Channel_10;
      break;   
#endif
#ifdef ADC_Channel_11      
    case 11:
      channel = ADC_Channel_11;
      break;
#endif   
#ifdef ADC_Channel_12      
    case 12:
      channel = ADC_Channel_12;
      break;
    case 13:
      channel = ADC_Channel_13;
      break;
    case 14:
      channel = ADC_Channel_14;
      break;
    case 15:
      channel = ADC_Channel_15;
      break;
 #endif     
#ifdef ADC_Channel_16
    case 16:
      channel = ADC_Channel_16;
      break;
#endif
#ifdef ADC_Channel_17
    case 17:
      channel = ADC_Channel_17;
      break;
#endif
#ifdef ADC_Channel_18
    case 18:
      channel = ADC_Channel_18;
      break;
#endif
#ifdef ADC_Channel_19
    case 19:
      channel = ADC_Channel_19;
      break;
#endif
    default:
      _Error_Handler("ADC: Unknown adc channel", (int)(CH_PIN_CHANNEL(function)));
      break;
  }
  
  return channel;
}

/**
  * @brief  Return ADC HAL internal channel linked to a PinName
  * @param  pin: specific PinName's for ADC internal. Value can be:
  *         PADC_TEMP, PADC_TEMP_ADC5, PADC_VREF, PADC_VBAT
  *         Note that not all of these values ​​may be available for all series.
  * @retval Valid HAL internal channel.
  */
uint32_t get_adc_internal_channel(PinName pin)
{
  uint32_t channel = 0;
  switch (pin) {
#if defined(ADC_Channel_TempSensor)
    case PADC_TEMP:
      channel = ADC_Channel_TempSensor;
      break;
#endif
#ifdef ADC_Channel_Vrefint
    case PADC_VREF:
      channel = ADC_Channel_Vrefint;
      break;
#endif
#ifdef ADC_Channel_Vbat
    case PADC_VBAT:
      channel = ADC_CHANNEL_VBAT;
      break;
#endif
    default:
      _Error_Handler("ADC: Unknown adc internal PiName", (int)(pin));
      break;
  }
  return channel;
}
#endif /* ADC_MODULE_ENABLED && !ADC_MODULE_ONLY */



#if defined(DAC_MODULE_ENABLED) && !defined(DAC_MODULE_ONLY)
/**
  * @brief  Return DAC HAL channel linked to a PinName
  * @param  pin: specific PinName's for ADC internal.
  * @retval Valid  channel
  */
uint32_t get_dac_channel(PinName pin)
{
  uint32_t function = pinmap_function(pin, PinMap_DAC);
  uint32_t channel = 0;
  switch (CH_PIN_CHANNEL(function)) 
  {
#ifdef DAC_Channel_1
    case 1:
      channel = DAC_Channel_1;
      break;
#endif
#ifdef DAC_Channel_2
    case 2:
      channel = DAC_Channel_2;
      break;
#endif
    default:
      _Error_Handler("DAC: Unknown dac channel", (int)(CH_PIN_CHANNEL(function)));
      break;
  }
  return channel;
}

////////////////////////// DAC INTERFACE FUNCTIONS /////////////////////////////
/**
  * @brief DAC  Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hdac: DAC pointer
  * @retval None
  */
void DAC_Clock_EN(DAC_TypeDef *dac)
{
    /* DAC Periph clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    /* Configure DAC GPIO pins */
    pinmap_pinout(g_current_pin, PinMap_DAC);
}


/**
  * @brief  This function will set the DAC to the required value
  * @param  port : the gpio port to use
  * @param  pin : the gpio pin to use
  * @param  value : the value to push on the adc output
  * @param  do_init : if set to 1 the initialization of the adc is done
  * @retval None
  */
void dac_write_value(PinName pin, uint32_t value, uint8_t do_init)
{
  DAC_TypeDef *pdac=NULL;
  DAC_InitTypeDef  DAC_InitType = {0};

  uint32_t dacChannel;

  pdac = (DAC_TypeDef *)pinmap_peripheral(pin, PinMap_DAC);
  if (pdac == NP) {
    return;
  }
  dacChannel = get_dac_channel(pin);

  if (do_init == 1) 
  {
   #if defined(CH32L10x)
    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOA, ENABLE );
	  RCC_PB1PeriphClockCmd(RCC_PB1Periph_DAC, ENABLE );   
   #else
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );
   #endif
    /*##-1- Configure the DAC peripheral */
    g_current_pin = pin;

    DAC_InitType.DAC_Trigger = DAC_Trigger_None;
    DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
    DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

    /*##-2- Configure DAC channelx */
    DAC_Init(dacChannel, &DAC_InitType);
    
  }
  /*##-3- Set DAC Channelx DHR register */
    switch (dacChannel)
    {
      case DAC_Channel_1:
        DAC_SetChannel1Data(DAC_Align_12b_R, value);
        break;
     case DAC_Channel_2:
        DAC_SetChannel2Data(DAC_Align_12b_R, value);
        break;
     default:
        DAC_SetChannel1Data(DAC_Align_12b_R, 0);
        DAC_SetChannel2Data(DAC_Align_12b_R, 0);
        break;
    }

  /*##-4- Enable DAC Channelx*/
  DAC_Cmd(dacChannel, ENABLE);
}


/**
  * @brief  This function will stop the DAC
  * @param  port : the gpio port to use
  * @param  pin : the gpio pin to use
  * @retval None
  */
void dac_stop(PinName pin)
{
  DAC_TypeDef *pdac=NULL;
  uint32_t dacChannel;

  pdac = (DAC_TypeDef *)pinmap_peripheral(pin, PinMap_DAC);
  if (pdac == NP) {
    return;
  }
  dacChannel = get_dac_channel(pin);
  //disable channel
  DAC_Cmd(dacChannel, DISABLE);

  DAC_DeInit( );
}
#endif //DAC_MODULE_ENABLED && !DAC_MODULE_ONLY




#if defined(ADC_MODULE_ENABLED) && !defined(ADC_MODULE_ONLY)
////////////////////////// ADC INTERFACE FUNCTIONS /////////////////////////////

/**
  * @brief ADC Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param padc: ADC handle pointer
  * @retval None
  */
void ADC_Clock_EN(ADC_TypeDef *padc)
{
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* ADC Periph clock enable */
  if (padc == ADC1) 
  {
     #ifdef RCC_APB2Periph_ADC1
      	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1	, ENABLE );	  //ENABLE ADC1 Clock
     #endif 

     #ifdef RCC_PB2Periph_ADC1
     RCC_PB2PeriphClockCmd(RCC_PB2Periph_ADC1	, ENABLE );	  //ENABLE ADC1 Clock for CH32L10x
     #endif
  }
#ifdef ADC2
  else if(padc == ADC2) 
  {
      #ifdef RCC_APB2Periph_ADC2
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2  , ENABLE );   //ENABLE ADC2 Clock 
      #endif 

      #ifdef RCC_PB2Periph_ADC2
      RCC_PB2PeriphClockCmd(RCC_PB2Periph_ADC2  , ENABLE );   //ENABLE ADC2 Clock for CH32L10x      
      #endif
  }
#endif
  /* Configure ADC GPIO pin */
  if (!(g_current_pin & PADC_BASE)) 
  {
      pinmap_pinout(g_current_pin, PinMap_ADC);
  }

  NVIC_EnableIRQ(ADC_IRQn);
}


/**
  * @brief  DeInitializes the ADC Moudle.
  * @param  padc: ADC handle
  * @retval None
  */
void ADC_Stop(ADC_TypeDef *padc)
{
    ADC_Cmd(padc,DISABLE); 
}

/**
  * @brief  This function will set the ADC to the required value
  * @param  pin : the pin to use
  * @param  resolution : resolution for converted data: 8/10/12
  * @retval the value of the adc
  */
uint16_t adc_read_value(PinName pin, uint32_t resolution, uint8_t gain)
{

  ADC_TypeDef *padc ;
  ADC_InitTypeDef ADC_InitStructure={0};

  uint16_t uhADCxConvertedValue = 0;
  uint32_t samplingTime = ADC_SAMPLINGTIME;
  uint32_t channel = 0;
  uint32_t bank = 0;

  if ((pin & PADC_BASE) && (pin < ANA_START))  //internal channle 
  {
    channel = get_adc_internal_channel(pin);
    samplingTime = ADC_SAMPLINGTIME_INTERNAL;
    padc = ADC1;
  } 
  else 
  {
    padc = (ADC_TypeDef *)pinmap_peripheral(pin, PinMap_ADC);
    channel = get_adc_channel(pin);
  }
  
  g_current_pin = pin;    
  ADC_Clock_EN(padc);  

#ifdef ADC_CLOCK_DIV
 #if !defined(CH32X035) 
  RCC_ADCCLKConfig(ADC_CLOCK_DIV);            /* (A)synchronous clock mode, input ADC clock divided */
 #else
  ADC_CLKConfig(padc, ADC_CLOCK_DIV);
 #endif 
#endif
#ifdef ADC_RESOLUTION_12B
#else
  (void) resolution;
#endif
#ifdef ADC_DataAlign_Right
  ADC_InitStructure.ADC_DataAlign            = ADC_DataAlign_Right;           /* Right-alignment for converted data */
#endif
  ADC_InitStructure.ADC_ScanConvMode         = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
 
  ADC_InitStructure.ADC_Mode                 = ADC_Mode_Independent;          /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  ADC_InitStructure.ADC_ContinuousConvMode   = DISABLE;
  ADC_InitStructure.ADC_NbrOfChannel         = 1; 
  ADC_InitStructure.ADC_ExternalTrigConv     = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_Pga                  = gain == 64 ? ADC_Pga_64 : gain == 16 ? ADC_Pga_16 : gain == 4 ? ADC_Pga_4 : ADC_Pga_1;
 #if !defined(CH32V00x) && !defined(CH32V10x)  && !defined(CH32VM00X)
  ADC_InitStructure.ADC_OutputBuffer         = ADC_OutputBuffer_Enable;
#endif
  ADC_Init(padc, &ADC_InitStructure);
  ADC_TempSensorVrefintCmd((pin & PADC_BASE) && (pin < ANA_START) ? ENABLE : DISABLE);
  padc->STATR = 0;
  
#if USE_FREERTOS
  ADC_ITConfig(padc, ADC_IT_EOC, ENABLE);
#endif

  ADC_Cmd(padc,ENABLE); 

  /*##-2- Configure ADC regular channel ######################################*/  
  ADC_RegularChannelConfig(padc, channel, ADC_REGULAR_RANK_1, samplingTime );

#if USE_FREERTOS
  conversionDone = false;
  adcWaitingTaskHandle = xTaskGetCurrentTaskHandle();
#endif
  /*##-3- Start the conversion process ####################*/
  ADC_SoftwareStartConvCmd(padc, ENABLE);
  /*##-4- Wait for the end of conversion #####################################*/
#if USE_FREERTOS
  while (!conversionDone) {
    ulTaskNotifyTake(1, pdMS_TO_TICKS(10));
  }
#else
  while(!ADC_GetFlagStatus(padc, ADC_FLAG_EOC));
#endif
  conversionDone = false;
  
  uhADCxConvertedValue = ADC_GetConversionValue(padc);
  ADC_ClearFlag(padc, ADC_FLAG_EOC | ADC_FLAG_STRT | ADC_FLAG_AWD);

  ADC_TempSensorVrefintCmd(DISABLE);
  ADC_Stop(padc);
  ADC_DeInit(padc);
  
#if USE_FREERTOS
  adcWaitingTaskHandle = nullptr;
  ADC_ITConfig(padc, ADC_IT_EOC, DISABLE);
#endif

#if defined(ADC_CTLR_ADCAL)
    uint16_t calibration_value = 0;
    if (padc == ADC1)
      calibration_value = calibration_value_adc1;
    else if (padc == ADC2)
      calibration_value = calibration_value_adc2;
  #if (ADC_RESOLUTION == 8)
    if(( calibration_value + uhADCxConvertedValue ) >= 255 )
    {
      return 255;
    }
    else if( ( calibration_value + uhADCxConvertedValue ) >= INT16_MAX )
    {
      return 0;
    }
    else
    {       
      return (uhADCxConvertedValue+calibration_value);
    }
  #endif
    #if (ADC_RESOLUTION == 10)
    if(( calibration_value + uhADCxConvertedValue ) >= 1023 )
    {
      return 1023;
    }
    else if( ( calibration_value + uhADCxConvertedValue ) >= INT16_MAX )
    {
      return 0;
    }
    else
    {       
      return (uhADCxConvertedValue+calibration_value);
    }
  #endif  
  #if (ADC_RESOLUTION == 12)
    if(( calibration_value + uhADCxConvertedValue ) >= 4095 )
    {
      return 4095;
    }
    else if( ( calibration_value + uhADCxConvertedValue ) >= INT16_MAX )
    {
      return 0;
    }
    else       
    {
      return (uhADCxConvertedValue+calibration_value);
    }
  #endif
#else
  return uhADCxConvertedValue;
#endif
}

#if defined(ADC_CTLR_ADCAL)
void perform_adc_calibration(ADC_TypeDef *padc) {
  ADC_Clock_EN(padc);  
  ADC_Cmd(padc,ENABLE);
#if USE_FREERTOS
  ADC_ITConfig(padc, ADC_IT_EOC, ENABLE);
#endif

#if !defined(CH32V00x)
  ADC_BufferCmd(padc, DISABLE);   //disable buffer
#endif

#if USE_FREERTOS
  adcWaitingTaskHandle = xTaskGetCurrentTaskHandle();
#endif


  __IO uint8_t  i, j;
  uint16_t      buf[10];
  __IO uint16_t t;
#if defined (CH32V20x_D6)
  __IO uint16_t p;
#endif

  for(i = 0; i < 10; i++){
      ADC_ResetCalibration(padc);
      while(ADC_GetResetCalibrationStatus(padc));
      ADC_StartCalibration(padc);
      while(ADC_GetCalibrationStatus(padc)) {
#if USE_FREERTOS
        ulTaskNotifyTake(1, pdMS_TO_TICKS(10));
#endif
      }
      buf[i] = padc->RDATAR;
  }
	ADC_BufferCmd(padc, ENABLE);   //enable buffer

#if USE_FREERTOS
  adcWaitingTaskHandle = nullptr;
  ADC_ITConfig(padc, ADC_IT_EOC, DISABLE);
#endif

    for(i = 0; i < 10; i++){
        for(j = 0; j < 9; j++){
            if(buf[j] > buf[j + 1])
            {
                t = buf[j];
                buf[j] = buf[j + 1];
                buf[j + 1] = t;
            }
        }
    }

#if defined (CH32V20x_D8) || defined (CH32V20x_D8W)
    t = 0;
    for( i = 0; i < 6; i++ ) {
        t += buf[i + 2];
    }

    t = ( t / 6 ) + ( ( t % 6 ) / 3 );

    int calibration_value = ( int16_t )( 2048 - ( int16_t )t );
#else
    t = 0;
    p = 0;
    /* 1024 */
    for(i = 0; i < 6; i++ ){
            if(buf[i+2] > 1536) break;
            t += buf[i+2];
    }

    if(i > 0){
            t = ( t / i ) + ( (( t % i )*2) / i );
    }
    else t = 1024;

    /* 2048 */
    j = 6-i;
    if(j > 0){
        for(; i < 6; i++ ){
                p += buf[i+2];
        }

        p = ( p / j ) + ( (( p % j )*2) / j );
    }
    else p = 2048;

    int calibration_value = ( int16_t )(((( int16_t )( 1024 - ( int16_t )t ) + ( int16_t )( 2048 - ( int16_t )p ))/2) + ((( int16_t )( 1024 - ( int16_t )t ) + ( int16_t )( 2048 - ( int16_t )p ))%2));
#endif
    if (padc == ADC1) {
      calibration_value_adc1 = calibration_value;
    } else if (padc == ADC2) {
      calibration_value_adc2 = calibration_value;
    }
}
#endif

#endif /* ADC_MODULE_ENABLED && !ADC_MODULE_ONLY*/



#if defined(TIM_MODULE_ENABLED) && !defined(TIM_MODULE_ONLY)
////////////////////////// PWM INTERFACE FUNCTIONS /////////////////////////////

/**
  * @brief  This function will set the PWM to the required value
  * @param  port : the gpio port to use
  * @param  pin : the gpio pin to use
  * @param  clock_freq : frequency of the tim clock
  * @param  value : the value to push on the PWM output
  * @retval None
  */
void pwm_start(PinName pin, uint32_t PWM_freq, uint32_t value, TimerCompareFormat_t resolution)
{
  TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(pin, PinMap_TIM);
  HardwareTimer *HT;
  TimerModes_t previousMode;
  uint32_t index = get_timer_index(Instance);
  if (HardwareTimer_Handle[index] == NULL) {
    HardwareTimer_Handle[index]->__this = new HardwareTimer((TIM_TypeDef *)pinmap_peripheral(pin, PinMap_TIM));
  }

  HT = (HardwareTimer *)(HardwareTimer_Handle[index]->__this);

  uint32_t pinFunction = pinmap_function(pin, PinMap_TIM);
  pin_function(pin, pinFunction);
  uint32_t channel = CH_PIN_CHANNEL(pinFunction);

  previousMode = HT->getMode(channel);
  if (previousMode != TIMER_OUTPUT_COMPARE_PWM1) {
    HT->setMode(channel, TIMER_OUTPUT_COMPARE_PWM1, pin);
  }
  HT->setOverflow(PWM_freq, HERTZ_FORMAT);
  HT->setCaptureCompare(channel, value, resolution);
  if (previousMode != TIMER_OUTPUT_COMPARE_PWM1) {
    HT->resume();
  }
}
/**
  * @brief  This function will disable the PWM
  * @param  port : the gpio port to use
  * @param  pin : the gpio pin to use
  * @retval None
  */
void pwm_stop(PinName pin)
{
  TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(pin, PinMap_TIM);
  HardwareTimer *HT;
  uint32_t index = get_timer_index(Instance);
  if (HardwareTimer_Handle[index] == NULL) {
    HardwareTimer_Handle[index]->__this = new HardwareTimer((TIM_TypeDef *)pinmap_peripheral(pin, PinMap_TIM));
  }

  HT = (HardwareTimer *)(HardwareTimer_Handle[index]->__this);
  if (HT != NULL) {
    delete (HT);
    HT = NULL;
  }
}
#endif /* TIM_MODULE_ENABLED && !TIM_MODULE_ONLY */

#ifdef __cplusplus
}
#endif

