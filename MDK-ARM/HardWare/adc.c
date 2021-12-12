//  #include "adc.h"
//	ADC_HandleTypeDef hadc1;
//	float ADCvalue;

///**
//  * @brief ADC1 Initialization Function
//  * @param None
//  * @retval None
//  */
//void MX_ADC1_Init(void)
//{

//  /* USER CODE BEGIN ADC1_Init 0 */

//  /* USER CODE END ADC1_Init 0 */

//  ADC_ChannelConfTypeDef sConfig = {0};

//  /* USER CODE BEGIN ADC1_Init 1 */

//  /* USER CODE END ADC1_Init 1 */
//  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
//  */
//  hadc1.Instance = ADC1;
//  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV16;//设置分频
//  hadc1.Init.Resolution = ADC_RESOLUTION_12B;//12位数据
//  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;//数据右对齐
//  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;//单通道 不扫描
//  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
//  hadc1.Init.LowPowerAutoWait = DISABLE;
//  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
//  hadc1.Init.ContinuousConvMode = ENABLE;//开启连续采集
//  hadc1.Init.NbrOfConversion = 1;//转换通道1路
//  hadc1.Init.DiscontinuousConvMode = DISABLE;
//  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;//软件开启
//  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
//  hadc1.Init.DMAContinuousRequests = DISABLE;
//  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
//  hadc1.Init.SamplingTimeCommon1 = LL_ADC_SAMPLINGTIME_160CYCLES_5;//设置采样率
//  hadc1.Init.SamplingTimeCommon2 = LL_ADC_SAMPLINGTIME_160CYCLES_5;
//  hadc1.Init.OversamplingMode = DISABLE;
//  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
//  if (HAL_ADC_Init(&hadc1) != HAL_OK)
//  {
//    ADCError_Handler();
//  }
//  /** Configure Regular Channel 
//  */
//  sConfig.Channel = ADC_CHANNEL_1;//对应的adc通道
//  sConfig.Rank = ADC_REGULAR_RANK_1;//多通道 配置到1
//  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
//  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//  {
//    ADCError_Handler();
//  }
//  /* USER CODE BEGIN ADC1_Init 2 */
//	//	HAL_ADC_Start(&hadc1);//ADC  start
//	  /* Run the ADC calibration */
//  if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)//校准开启
//  {
//    /* Calibration Error */
//    ADCError_Handler();
//  }
//	  /* Start ADC group regular conversion with IT */
//    if (HAL_ADC_Start(&hadc1) != HAL_OK)//开启ADC
//    {
//      /* ADC conversion start error */
//      ADCError_Handler();
//    }
//  /* USER CODE END ADC1_Init 2 */

//}


//void ADC_GETValue(void)
//{
//	   int adctemp;
//	  HAL_ADC_PollForConversion(&hadc1,10);    //等待转换完成，第二个参数表示超时时间，单位ms        
//		if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
//		{
//      adctemp = HAL_ADC_GetValue(&hadc1);
//      ADCvalue =((3.3*adctemp)/4095.0);//转换成电压值
//		}

//}
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)//中断接收程序重写
//{
//   int adctemp;
//  /* Prevent unused argument(s) compilation warning */
//  UNUSED(hadc);
//	adctemp = HAL_ADC_GetValue(hadc);
//  ADCvalue =((3.3*adctemp)/4095.0);//转换成电压值
//  /* NOTE: This function Should not be modified, when the callback is needed,
//           the HAL_UART_TxCpltCallback could be implemented in the user file
//*/


//}
//void ADCError_Handler(void)
//{
//  /* USER CODE BEGIN Error_Handler_Debug */
//  /* User can add his own implementation to report the HAL error return state */

//  /* USER CODE END Error_Handler_Debug */
//}
