/*
 * Adc.h
 *
 *  Created on: Aug 31, 2021
 *      Author: Yip Sai Wei
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <BaseAddr/BaseAddress.h>
#include  "IO.h"
#include  "Rcc.h"

#define adc1  ((Adc*)Adc1BaseAddress)

typedef struct  Adc_t  Adc;
struct  Adc_t {
  _IO_ int32_t  SR;
  _IO_ int32_t  CR[2];
  _IO_ int32_t  SMPR[2];
  _IO_ int32_t  JOFR[4];
  _IO_ int32_t  HTR;
  _IO_ int32_t  LTR;
  _IO_ int32_t  SQR[3];
  _IO_ int32_t  JSQR;
  _IO_ int32_t  JDR[4];
  _IO_ int32_t  DR;
  _IO_ int32_t  RESERVED[101];
  _IO_ int32_t  CCR;
};

typedef enum{
  //CR1
  EOC_INT_EN = 1 << 5,
  ANALOG_WATCHDOG_INT_EN = 1 << 6,
  INJECTED_CHN_INT_EN = 1 << 7,
  SCAN_MODE_EN = 1 << 8,
  WATCHDOG_1_CHANNEL_SCAN_EN = 1 << 9,
  AUTO_INJECTED_GROUP_CONVERSION_EN = 1 << 10,
  DISCONTI_MODE_ON_REGULAR_CHN_EN = 1 << 11,
  DISCONTI_MODE_ON_INJECTED_CHN_EN = 1 << 11,
  ANALOG_WATCHDOG_INJECTED_CHN_EN = 1 << 22,
  ANALOG_WATCHDOG_REGULAR_CHN_EN = 1 << 23,
  RESOLUTION_12_BIT = 0 << 24,
  RESOLUTION_10_BIT = 1 << 24,
  RESOLUTION_8_BIT = 2 << 24,
  RESOLUTION_6_BIT = 3 << 24,
  OVERRUN_INT_EN = 1 << 26,

  //CR2
  ADC_CONVERTER_ON = 1LL << (0 + 32),
  CONTI_CONVERSION = 1LL << (1 + 32),
  DMA_MODE_EN = 1LL << (8 + 32),
  REQUEST_DMA_WHEN_DATA_CONVERTED_AND_DMA_EN = 1LL << (9 + 32),
  SET_EOC_BIT_AFTER_REGULAR_CONVERSION_ENDED = 1LL << (10 + 32),
  LEFT_ALLIGNMENT = 1LL << (11 + 32),
  INJECTED_TRIG_DETECTION_RISING_EDGE = 1LL << (20 + 32),
  INJECTED_TRIG_DETECTION_FALLING_EDGE = 2LL << (20 + 32),
  INJECTED_TRIG_DETECTION_BOTH_EDGES = 3LL << (20 + 32),
  INJECTED_CHN_START_CONVERT = 1LL << (22 + 32),
  REGULAR_TRIG_DETECTION_RISING_EDGE = 1LL << (28 + 32),
  REGULAR_TRIG_DETECTION_FALLING_EDGE = 2LL << (28 + 32),
  REGULAR_TRIG_DETECTION_BOTH_EDGES = 3LL << (28 + 32),
  REGULAR_CHN_START_CONVERT = 1LL << (30 + 32),

  INJECTED_EXT_EVENT_SEL_TIM_1_CC4 = 0LL << (16 + 32),
  INJECTED_EXT_EVENT_SEL_TIM_1_TRGO = 1LL << (16 + 32),
  INJECTED_EXT_EVENT_SEL_TIM_2_CC1 = 2LL << (16 + 32),
  INJECTED_EXT_EVENT_SEL_TIM_2_TRGO = 3LL << (16 + 32),
  INJECTED_EXT_EVENT_SEL_TIM_3_CC2 = 4LL << (16 + 32),
  INJECTED_EXT_EVENT_SEL_TIM_3_CC4 = 5LL << (16 + 32),
  INJECTED_EXT_EVENT_SEL_TIM_4_CC1 = 6LL << (16 + 32),
  INJECTED_EXT_EVENT_SEL_TIM_4_CC2 = 7LL << (16 + 32),
  INJECTED_EXT_EVENT_SEL_TIM_4_CC3 = 8LL << (16 + 32),
  INJECTED_EXT_EVENT_SEL_TIM_4_TRGO = 9LL << (16 + 32),
  INJECTED_EXT_EVENT_SEL_TIM_5_CC4 = 10LL << (16 + 32),
  INJECTED_EXT_EVENT_SEL_TIM_5_TRGO = 11LL << (16 + 32),
  INJECTED_EXT_EVENT_SEL_EXTI_LINE_5 = 15LL << (16 + 32),

  REGULAR_EXT_EVENT_SEL_TIM_1_CC1 = 0LL << (24 + 32),
  REGULAR_EXT_EVENT_SEL_TIM_1_CC2 = 1LL << (24 + 32),
  REGULAR_EXT_EVENT_SEL_TIM_1_CC3 = 2LL << (24 + 32),
  REGULAR_EXT_EVENT_SEL_TIM_2_CC2 = 3LL << (24 + 32),
  REGULAR_EXT_EVENT_SEL_TIM_2_CC3 = 4LL << (24 + 32),
  REGULAR_EXT_EVENT_SEL_TIM_2_CC4 = 5LL << (24 + 32),
  REGULAR_EXT_EVENT_SEL_TIM_2_TRGO = 6LL << (24 + 32),
  REGULAR_EXT_EVENT_SEL_TIM_3_CC1 = 7LL << (24 + 32),
  REGULAR_EXT_EVENT_SEL_TIM_3_TRGO = 8LL << (24 + 32),
  REGULAR_EXT_EVENT_SEL_TIM_4_CC4 = 9LL << (24 + 32),
  REGULAR_EXT_EVENT_SEL_TIM_5_CC1 = 10LL << (24 + 32),
  REGULAR_EXT_EVENT_SEL_TIM_5_CC2 = 11LL << (24 + 32),
  REGULAR_EXT_EVENT_SEL_TIM_5_CC3 = 12LL << (24 + 32),
  REGULAR_EXT_EVENT_SEL_EXTI_LINE_5 = 15LL << (24 + 32),
}adcConfig;

typedef enum{
  ADC_3_CYCLES,
  ADC_15_CYCLES,
  ADC_28_CYCLES,
  ADC_56_CYCLES,
  ADC_84_CYCLES,
  ADC_112_CYCLES,
  ADC_144_CYCLES,
  ADC_480_CYCLES,
}adcSmplingTime;

void  adcSetChannelSamplingTime(Adc *adc, int channel, adcSmplingTime  samplingTime);
void  adcSetChannelSequence(Adc *adc, int channels[], int  length);
void  adcConfiguration(Adc  *adc, adcConfig config);

#endif /* INC_ADC_H_ */
