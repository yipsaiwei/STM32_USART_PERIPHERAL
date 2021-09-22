/*
 * Adc.c
 *
 *  Created on: Sep 4, 2021
 *      Author: Yip Sai Wei
 */

#include  "Adc.h"

void  adcSetChannelSamplingTime(Adc *adc, int channel, adcSmplingTime  samplingTime){
  if(channel < 9)
    adc->SMPR[1] |= samplingTime << (3 * channel);
  else
    adc->SMPR[0] |= samplingTime << (3 * (channel - 10));
}

void  adcSetChannelSequence(Adc *adc, int channels[], int  length){
  adc->SQR[0] |= length << 20;
  for(int i = 0; i < length; i++){
    if(i < 6)
      adc->SQR[2] |= channels[i] << (i * 5);
    else if(i < 13)
      adc->SQR[1] |= channels[i] << ((i - 6) * 5);
    else
      adc->SQR[0] |= channels[i] << ((i - 13) * 5);
  }
}

void  adcConfiguration(Adc  *adc, adcConfig config){
  adc->CR[0] &= ~(0xFFFFFFFF);
  adc->CR[0] |= (config & 0xFFFFFFFF);

  adc->CR[1] &= ~(0xFFFFFFFF);
  adc->CR[1] |= (config >> 32);
}
