#ifndef __ADC_H
#define __ADC_H

#include "stm32g0xx_hal.h"
void ADCError_Handler(void);
void MX_ADC1_Init(void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);
void ADC_GETValue(void);
extern 	float ADCvalue;

#endif
