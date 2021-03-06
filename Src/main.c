/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "stdlib.h"
#include "string.h"
#include "stdlib.h"
#include "EC200S.h"
#include "command.h"
//#include "AHT20.h"
//#include "oled.h"	
//#include "adc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Captured Value */
__IO uint32_t            uwIC2Value = 0;
/* Duty Cycle Value */
__IO uint32_t            uwDutyCycle = 0;
/* Frequency Value */
__IO uint32_t            uwFrequency = 0;
/* USER CODE END PV */
__IO uint32_t            uwValue = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static int  Check_Timeout( SW_TIMER_STRUCT * stimer );
static void SWTimer_Start( SW_TIMER_STRUCT * stimer );
static void Timer_Init(SW_TIMER_STRUCT timer[TIMER_MAX]);
static void Led_Blink(SW_TIMER_STRUCT * stimer);
static u32  Gun_Check(SW_TIMER_STRUCT * stimer, SCAN_DATA_STRUCT * Data_Scan);
static void State_Machine(u32 cur_state, u32 * next_state_ptr, SCAN_DATA_STRUCT * Data_Scan, SW_TIMER_STRUCT timer[TIMER_MAX], u32 inject_value, u32 * cur_value, u8 * flag);
static void Pub_MsgData(u8 topic);
static void Heart_Beat( SW_TIMER_STRUCT * stimer );
static u8   Login_Decode(void);
static void Task_Decode(u8 * step);
//void OLED_SHOWAPP(void);
int  temp,humi;
u8                       d_step;

/* USER CODE BEGIN PFP */
SW_TIMER_STRUCT  static  timer[TIMER_MAX];
UREA_PARA_STRUCT         urea_para;
TASK_PARA_STRUCT         task_para;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
TIM_HandleTypeDef htim1;
/* USER CODE END 0 */
/* USER CODE END 0 */
void OPEN_CAT1(void)
{
	char *strx;
		printf("AT\r\n"); 
		HAL_Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//??????OK
		printf("AT\r\n"); 
		HAL_Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//??????OK
	if(strx==NULL)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3,GPIO_PIN_SET);//??????PWRKEY
		HAL_Delay(600);
		HAL_Delay(600);
		HAL_Delay(600);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3,GPIO_PIN_RESET);//??????????????????
//    IWDG_Feed();//??????
	}
	printf("AT\r\n"); 
		HAL_Delay(300);
//	 IWDG_Feed();//??????
		strx=strstr((const char*)RxBuffer,(const char*)"OK");//??????OK
		while(strx==NULL)
		{
				Clear_Buffer();	
				printf("AT\r\n"); 
				HAL_Delay(100);
			  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12);
				strx=strstr((const char*)RxBuffer,(const char*)"OK");//??????OK
		}
     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12,GPIO_PIN_RESET);
//		 IWDG_Feed();//??????
}


/**
  * @brief  The application entry point.
  * @retval int
  */
int a = 0;
int rec_value=0;
int main(void)
{

	u8 ret = 0;
	u8 Step = 0;
	u32 inject_val = 0;
  u32 cur_val = 0;
	u32 CT_data[2];
	u32 len;
	u32 static	cur_state = LOGIN;																																									
	u32 static	next_state = LOGIN;
  u8  static  send_flag = 0;	
	SCAN_DATA_STRUCT	static		Gun_Scan;
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  SystemClock_Config();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
//	MX_IWDG_Init();
  MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_TIM1_Init();
	HAL_Delay(500);
	
  /* USER CODE BEGIN 2 */
	 OPEN_CAT1();
  /* USER CODE END 2 */
	EC200S_Init();
	//MQTT_Init();//MQTT?????????
	
	/*????????????*/
	                                    //????????? ??????
	Uart1_SendStr("Welcome! \r\n");		  //??????????????????
	MQTT_Init();                        //?????????????????????
	                                    //???????????????????????????
	                                    //????????????
	Timer_Init(timer);
	
	send_flag =  SEND_EN;
	d_step    =  TASK_ID;
	Uart1_SendStr("Welcome! \r\n");	
  while (1)
  {
		
		Led_Blink(&timer[T_LED]);         //LED??????
		//Heart_Beat(&timer[T_HBT]);
		Gun_Check(&timer[T_2MS], &Gun_Scan);  //?????????????????????
		
		cur_state = next_state;
		State_Machine(cur_state, &next_state, &Gun_Scan, timer, inject_val, &cur_val, &send_flag);

		//if(strcmp(READY_FILL,"jzj/0/readyfill"));
//				Pub_Msgdata();//????????????
//				HAL_Delay(300);
//				EC200S_RECData();//??????????????????????????????
//				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12);
//				IWDG_Feed();//??????
  }
  
}
int m=0;
static void Heart_Beat( SW_TIMER_STRUCT * stimer)
{
	//if(Check_Timeout(&stimer[T_HBT]))
	if(Check_Timeout(stimer))
	{m++;
		char *strx;
		uint8_t send_jason[300];
		uint16_t data_len;
		EC200U_GETGNSSdata();//??????GPS??????
		data_len=Mqttaliyun_Savedata(send_jason,temp/10,humi/10);
		printf("AT+QMTPUBEX=0,0,0,0,\"%s\",%d\r\n","jzj/0/cycledata",data_len);
		HAL_Delay(100);
	  printf("%s",send_jason);
		HAL_Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUBEX: 0,0,0");//  +QMTPUBEX: 0,0,0
		while(strx==NULL)
		{
			strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUBEX: 0,0,0");//  +QMTPUBEX: 0,0,0
		}
		EC200S_RECData();//??????????????????????????????
		HAL_Delay(500);	HAL_Delay(500);
		SWTimer_Start(stimer);
	}
}

u8 json[100];
u8 top[30];

static void Pub_MsgData(u8 topic)
{
	char *strx;

	if(READY_FILL == topic)
	{
		sprintf((char *)json,"{\"message_id\":\"fehjijiadaiefn\",\"timestamp\":\"12345678\"}");
		sprintf((char *)top,"jzj/%d/readyfill",id);
	}
	
	printf("AT+QMTPUBEX=0,0,0,0,\"%s\",%d\r\n",top,strlen((char *)json));
	HAL_Delay(100);
	printf("%s",json);
	HAL_Delay(300);
	
	while(strx==NULL)
	{
		strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUBEX: 0,0,0");//  +QMTPUBEX: 0,0,0
	}	
	HAL_Delay(100);
	Clear_Buffer();
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks 
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);//LEDstatus
	  /*Configure GPIO pin Output Level */
	  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
	  /*Configure GPIO pin : PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);//PWRKEY??????
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_SET);
	  /*Configure GPIO pin : PB2 PB3 B4 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);//
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
	  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);//BEEP
	/*Configure GPIO pin : PB12 PB13 PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_4;      //???????????????
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);//

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  //htim1.Init.Prescaler = 0;
	htim1.Init.Prescaler = 63;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xFFFF;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI2FP2;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchro(&htim1, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
	
	 /*## Start the Input Capture in interrupt mode ##########################*/
  if (HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }

  /*## Start the Input Capture in interrupt mode ##########################*/
  if (HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  /* USER CODE END 2 */

}

/**
  * @brief  Input Capture callback in non blocking mode 
  * @param  htim : TIM IC handle
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
  {uwValue++;
    /* Get the Input Capture value */
    uwIC2Value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

    if (uwIC2Value != 0)
    {
      /* Duty cycle computation */
      uwDutyCycle = ((HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1)) * 100) / uwIC2Value;

      /* uwFrequency computation
      TIM1 counter clock = (System Clock) */
      uwFrequency = ( HAL_RCC_GetSysClockFreq()/64  ) / uwIC2Value;

    }
    else
    {
      uwDutyCycle = 0;
      uwFrequency = 0;
    }
  }
}

/* USER CODE END 4 */
static void Timer_Init(SW_TIMER_STRUCT			timer[TIMER_MAX])
{
	timer[T_2MS].count_en = ENABLE;
	timer[T_2MS].timeout_ms = 2;
	timer[T_LED].count_en = ENABLE;
	timer[T_LED].timeout_ms = 1000;
	timer[T_HAF].count_en = DISABLE;
	timer[T_HAF].timeout_ms = 2000;
  timer[T_HBT].count_en = ENABLE;
	timer[T_HBT].timeout_ms = 60000;
	timer[T_3].count_en = ENABLE;
	timer[T_3].timeout_ms = 2000;
}

static int Check_Timeout( SW_TIMER_STRUCT * stimer )
{
	int cur_ms, passed_ms;
	
	if(stimer->count_en ==ENABLE)
	{	
		//cur_ms = g_time_ms;
		cur_ms = HAL_GetTick();
		if(stimer->timeout_over == 0)
		{
			passed_ms = (cur_ms - stimer->old_ms)&0x0000ffff;
			if (passed_ms >= stimer->timeout_ms)
				stimer->timeout_over = 1;
			else
				stimer->timeout_over = 0;
		}
	}
	else
		stimer->timeout_over = 0;
	return stimer->timeout_over;	
}

void SWTimer_Start( SW_TIMER_STRUCT * stimer )
{
	if(stimer->count_en == ENABLE)
	{
		stimer->old_ms = HAL_GetTick();
		stimer->timeout_over = 0;
	}
}

static void Led_Blink(SW_TIMER_STRUCT * stimer)
{
	if(Check_Timeout(stimer))  //input??????
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12);
		SWTimer_Start(stimer);
	}
}

static u32 Gun_Check(SW_TIMER_STRUCT * stimer, SCAN_DATA_STRUCT * Data_Scan)
{
	if(Check_Timeout(stimer))  //input??????
	{
		
		u32 bit = Data_Scan->data;
	
	  //collect input signal
	  Data_Scan->data_f1  = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
	  //or
	  Data_Scan->data_and = Data_Scan->data_f1&Data_Scan->data_f2&Data_Scan->data_f3&Data_Scan->data_f4;
	  //and
	  Data_Scan->data_or  = Data_Scan->data_f1|Data_Scan->data_f2|Data_Scan->data_f3|Data_Scan->data_f4;
	
		if(Data_Scan->data_and)   								//?????????????????????1
			bit = 1;
		else if((Data_Scan->data_or&bit) == 0)		//??????????????????????????????1???
			bit = 0;
	
	  Data_Scan->data_f4  = Data_Scan->data_f3;
	  Data_Scan->data_f3  = Data_Scan->data_f2;
	  Data_Scan->data_f2  = Data_Scan->data_f1;
		Data_Scan->data = bit;
		
		SWTimer_Start(stimer);
	}
}

int ddd = 0;
static void State_Machine(u32 cur_state, u32 * next_state_ptr, SCAN_DATA_STRUCT * Data_Scan, SW_TIMER_STRUCT timer[TIMER_MAX], u32 inject_value, u32 * cur_value, u8 * flag)
{
	switch(cur_state)
	{
		case LOGIN:
		{
			if(Login_Decode())
			{
				*next_state_ptr = READY;
			}
		  break;
		}
		case READY:
    {a=1;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET); //??????(PB12)????????????(PB13)????????????(PB14)
			
			if(SEND_EN == *flag)
			{
				Uart1_SendStr("Ready \r\n");        //??????????????????
				Pub_MsgData(READY_FILL);
				*flag = SEND_OVER;
			}
			
			Task_Decode(&d_step);
			
			if(Data_Scan->data == GUN_ON)         //????????????????????????  
			{
				if(rec_value == 1)
				{
					*next_state_ptr = STAY_INJECT;
					*flag = SEND_EN;
				}
			}
			break;
		}
		case STAY_INJECT:
		{a=2;
			//Uart1_SendStr("Stay_Inject \r\n");  //???????????????
			if(Data_Scan->data == GUN_OFF)  //????????????????????????
			{
				*cur_value =  uwIC2Value;
				*next_state_ptr = HALF_INJECT;
			}
      break;			
		}
		case HALF_INJECT:
		{a=3;
			Uart1_SendStr("Injecting \r\n");  //??????????????????
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_14, GPIO_PIN_SET); //??????(PB12)????????????(PB14)
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET); //?????????(PB13)
			
			if((uwFrequency>=FREQ_5K) && (uwFrequency<=FREQ_6K))
			{
				if(Check_Timeout(&timer[T_HAF]))
				{
					timer[T_HAF].count_en = DISABLE;
					*next_state_ptr = FULL_INJECT;
				}
			}
		  else
			{
				timer[T_HAF].count_en = ENABLE;
			  SWTimer_Start(&timer[T_HAF]);
			}
			
		  if(Data_Scan->data == GUN_OFF)  //?????????????????????
      {
				*next_state_ptr = FINISH;
			}
			
			break;
		}
		case FULL_INJECT:
		{a=4;
			//Uart1_SendStr("Injecting \r\n");  //??????????????????
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_SET); //??????(PB12),?????????(PB13)????????????(PB14)
			if((uwIC2Value - *cur_value) > (inject_value - TAIL_VALUE))
			{
				*next_state_ptr = TAIL_INJECT;
			}
			
		  if(Data_Scan->data == GUN_OFF)  //?????????????????????
      {
				*next_state_ptr = FINISH;
			}			
			
			break;
		}
		case TAIL_INJECT:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_SET); //??????(PB12),?????????(PB13)
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); //?????????(PB14)
		  if((uwIC2Value - *cur_value) > (inject_value))
			{
				*next_state_ptr = FINISH;
			}
			
		  if(Data_Scan->data == GUN_OFF)  //?????????????????????
      {
				*next_state_ptr = FINISH;
			}			
			
		break;
		case FINISH:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_SET); //??????(PB12),?????????(PB13),?????????(PB14)
		  if(Data_Scan->data == GUN_OFF)  //????????????????????????
      {
				*next_state_ptr = READY;
			}				
		break;
		default:
		break;		
	}
}

static u8 Login_Decode(void)
{
	char  *str1;
	char  *str2;
	char  *str3;
	char  buf[30];
	char  buf1[30];
	char  price[10];
	int i =0;
	int j =0;
	char sss = ',';
	char dot = '.';
	u8 dot_flag = 0;
	u8 flag = 1;
	
  while(flag)
  { 
	  str1=strstr((const char*)RxBuffer,(const char*)"message_id");      //??????id
	  str2=strchr((const char*)str1,':');
	  str3=strchr((const char*)str1,',');
	  memcpy(buf,str2+3,30);

	  for(i=0;i<30;i++)
   {
		 if(buf[i]==sss)
		 {
			 j=i;
			 memcpy(buf1,buf,j);
			 memcpy(&urea_para.message_id,buf1,i-1);
		 }
	 }

	 str1=strstr((const char*)RxBuffer,(const char*)"timestamp");      //?????? pulse_equ
	 str2=strchr((const char*)str1,':');
	 str3=strchr((const char*)str1,',');
	 memcpy(buf,str2+2,30);

	 for(i=0;i<30;i++)
	 {
		 if(buf[i]==sss)
		 {
			 int sum = 0;
			 for(j=0;j<i;j++)
			 {
				 sum = sum*10;
				 sum = sum + (buf[j]-0x30);
			 }
			 urea_para.timestamp = sum;
			 break;
		 }
	 }
	 
//	 str1=strstr((const char*)RxBuffer,(const char*)"timestamp");      //?????? timestamp
//	 str2=strchr((const char*)str1,':');
//	 str3=strchr((const char*)str1,',');
//	 memcpy(buf,str2+3,30);

//	 for(i=0;i<30;i++)
//	 {
//		 if(buf[i]==sss)
//		 {
//			 j=i;
//			 memcpy(buf1,buf,i);
//			 memcpy(&urea_para.timestamp,buf1,i-1);
//		 }
//	 }

//	 str1=strstr((const char*)RxBuffer,(const char*)"pulse_equ");      //?????? pulse_equ
//	 str2=strchr((const char*)str1,':');
//	 str3=strchr((const char*)str1,',');
//	 memcpy(buf,str2+1,30);

	 for(i=0;i<20;i++)
	 {
		 if(buf[i]==sss)
		 {
			 int sum = 0;
			 for(j=0;j<i;j++)
			 {
				 sum = sum*10;
				 sum = sum + (buf[j]-0x30);
			 }
			 urea_para.pulse_equ = sum;
		 }
	 }
	 
	 str1=strstr((const char*)RxBuffer,(const char*)"open_price");      //?????? open_price
	 str2=strchr((const char*)str1,':');
	 str3=strchr((const char*)str1,',');
	 memcpy(buf,str2+1,30);

	 for(i=0;i<20;i++)
	 {
		 if(buf[i]==sss)
		 {
			 memcpy(price,buf,i);
			 dot_flag = 0;
			 float sum=0;
			 float div=0;
			 int dot_sum = 0;
			 for(j=0;j<i;j++)
			 {
				 if(price[j]==dot)
				 {
					 dot_flag = 1;
				 }
 
				 if(dot_flag == 0)
				 {
					 sum = sum*10;
					 sum = sum + (price[j] - 0x30);
				 }
				 else if(dot_flag == 1)
				 {
					 if(dot_sum == 0)
					 {
						 dot_sum=1;
					 }
					 else
					 {
						 dot_sum = dot_sum*10;
						 div = (float)(price[j]-0x30)/(float)dot_sum;
					 }
					 sum = sum + div;
				 }
				 urea_para.open_price = sum;
			 }
		 }
	 }

	 str1=strstr((const char*)RxBuffer,(const char*)"QR_Code");      //?????? QR_Code
	 str2=strchr((const char*)str1,':');
	 str3=strchr((const char*)str1,',');
	 memcpy(buf,str2+3,30);

	 for(i=0;i<30;i++)
	 {
		 if(buf[i]==sss)
		 {
			 j=i;
			 memcpy(buf1,buf,i);
			 memcpy(&urea_para.QR_Code,buf1,i-1);
		 }
	 }
	 
	 str1=strstr((const char*)RxBuffer,(const char*)"conctr");      //?????? conctr
	 str2=strchr((const char*)str1,':');
	 str3=strchr((const char*)str1,',');
	 memcpy(buf,str2+1,30);

	 for(i=0;i<20;i++)
	 {
		 if(buf[i]==0x0A)
		 {
			 memcpy(price,buf,i);
			 dot_flag = 0;
			 float sum=0;
			 float div=0;
			 int dot_sum = 0;
			 for(j=0;j<i;j++)
			 { 

				 if(price[j]==dot)
				 {
					 dot_flag = 1;
				 }
				 
				 if(dot_flag == 0)
				 {
					 sum = sum*10;
					 sum = sum + (price[j] - 0x30);
				 }
				 else if(dot_flag == 1)
				 {
					 if(dot_sum == 0)
					 {
						 dot_sum=1;
					 }
					 else
					 {
						 dot_sum = dot_sum*10;
						 div = (float)(price[j]-0x30)/(float)dot_sum;
					 }
					 sum = sum + div;
				 }
				 urea_para.conctr = sum;
			 }
		 }
	 }			 

	 str1=strstr((const char*)RxBuffer,(const char*)"status");      //?????? pulse_equ
	 str2=strchr((const char*)str1,':');
	 str3=strchr((const char*)str1,',');
	 memcpy(buf,str2+1,30);

	 for(i=0;i<5;i++)
	 {
		 if(buf[i]==sss)
		 {
			 int sum = 0;
			 for(j=0;j<i;j++)
			 {
				 sum = sum*10;
				 sum = sum + (buf[j]-0x30);
			 }
			 urea_para.status = sum;
		 }
	 }
	 
	 if((urea_para.status==1)&&(*urea_para.message_id!=0)&&(urea_para.open_price!=0)&& \
			 (urea_para.timestamp!=0)&&(urea_para.conctr!=0)&&(*urea_para.QR_Code!=0)&&(urea_para.pulse_equ!=0))
	 {
			 flag = 0;
	 }				 
	 ddd=1;
 }
	
  Clear_Buffer(); 
	return 1;
}

static void Task_Decode(u8 * step)
{
	char  *str1;
	char  *str2;
	char  *str3;
	char  buf[30];
	char  buf1[30];
	char  price[10];
	int i =0;
	int j =0;
	char sss = ',';
	char dot = '.';
	u8 dot_flag = 0;
	u8 flag = 1;

  if(*step == TASK_ID)
	{
		str1=strstr((const char*)RxBuffer,(const char*)"message_id");      //??????id
	  str2=strchr((const char*)str1,':');
	  str3=strchr((const char*)str1,',');
	  memcpy(buf,str2+3,30);
		
		for(i=0;i<30;i++)
    {
		  if(buf[i]==sss)
		  {
			  //j=i;
			  memcpy(buf1,buf,i);
			  memcpy(&task_para.message_id,buf1,i-1);
				break;
		  }
	  }
	 
	  if(*task_para.message_id != 0)
		{
			*step = TASK_TIME;
		}
	}
	else if(*step == TASK_TIME)
	{
		 str1=strstr((const char*)RxBuffer,(const char*)"timestamp");      //?????? pulse_equ
		 str2=strchr((const char*)str1,':');
		 str3=strchr((const char*)str1,',');
		 memcpy(buf,str2+2,30);

		 for(i=0;i<30;i++)
		 {
			 if(buf[i]==sss)
			 {
				 int sum = 0;
				 for(j=0;j<i;j++)
				 {
					 sum = sum*10;
					 sum = sum + (buf[j]-0x30);
				 }
				 task_para.timestamp = sum;
				 break;
			 }
		 }

    if(task_para.timestamp != 0)
		{
			*step = TASK_VLM;
		}
	}
	else if(*step == TASK_VLM)
	{
		 str1=strstr((const char*)RxBuffer,(const char*)"task_vlm");      //?????? pulse_equ
		 str2=strchr((const char*)str1,':');
		 str3=strchr((const char*)str1,',');
		 memcpy(buf,str2+1,30);

		 for(i=0;i<30;i++)
		 {
			 if(buf[i]==sss)
			 {
				 int sum = 0;
				 for(j=0;j<i;j++)
				 {
					 sum = sum*10;
					 sum = sum + (buf[j]-0x30);
				 }
				 task_para.task_vlm = sum;
				 break;
			 }
		 }
     
		 if(task_para.task_vlm != 0)
		{
			*step = TASK_TYPE;
		}
	}
	else if(*step == TASK_TYPE)
	{
		 str1=strstr((const char*)RxBuffer,(const char*)"task_type");      //?????? pulse_equ
		 str2=strchr((const char*)str1,':');
		 str3=strchr((const char*)str1,',');
		 memcpy(buf,str2+1,30);

		 for(i=0;i<6;i++)
		 {
			 if(buf[i]==sss)
			 {
				 int sum = 0;
				 for(j=0;j<i;j++)
				 {
					 sum = sum*10;
					 sum = sum + (buf[j]-0x30);
				 }
				 task_para.task_type = sum;
				 break;
			 }
		 }
     
		 if(task_para.task_type != 0)
		{
			*step = TASK_NO;
		}		
	}
	else if(*step == TASK_NO)
	{
		str1=strstr((const char*)RxBuffer,(const char*)"task_no");       //?????? task_no
	  str2=strchr((const char*)str1,':');
	  str3=strchr((const char*)str1,',');
	  memcpy(buf,str2+3,30);
		
		for(i=0;i<20;i++)
    {
		  if(buf[i]==sss)
		  {
			  memcpy(buf1,buf,i);
			  memcpy(&task_para.task_no,buf1,i-1);
				break;
		  }
	  }
	 
	  if(*task_para.task_no != 0)
		{
			*step = TASK_BUSI_TAIL;
		}
	}
	else if(*step == TASK_BUSI_TAIL)
	{
		 str1=strstr((const char*)RxBuffer,(const char*)"busi_tail");      //?????? TASK_BUSI_TAIL
		 str2=strchr((const char*)str1,':');
		 str3=strchr((const char*)str1,',');
		 memcpy(buf,str2+1,30);

		 for(i=0;i<30;i++)
		 {
			 if(buf[i]==sss)
			 {
				 int sum = 0;
				 for(j=0;j<i;j++)
				 {
					 sum = sum*10;
					 sum = sum + (buf[j]-0x30);
				 }
				 task_para.busi_tail = sum;
				 break;
			 }
		 }
     
		 if(task_para.busi_tail != 0)
		{
			*step = TASK_PRICE;
		}
	}
	else if(*step == TASK_PRICE)
	{
		 str1=strstr((const char*)RxBuffer,(const char*)"price");      //?????? price
		 str2=strchr((const char*)str1,':');
		 str3=strchr((const char*)str1,',');
		 memcpy(buf,str2+1,30);

		 for(i=0;i<10;i++)
		 {
			 if(buf[i]==sss)
			 {
				 memcpy(price,buf,i);
				 dot_flag = 0;
				 float sum=0;
				 float div=0;
				 int dot_sum = 0;
				 for(j=0;j<i;j++)
				 {
					 if(price[j]==dot)
					 {
						 dot_flag = 1;
					 }
	 
					 if(dot_flag == 0)
					 {
						 sum = sum*10;
						 sum = sum + (price[j] - 0x30);
					 }
					 else if(dot_flag == 1)
					 {
						 if(dot_sum == 0)
						 {
							 dot_sum=1;
						 }
						 else
						 {
							 dot_sum = dot_sum*10;
							 div = (float)(price[j]-0x30)/(float)dot_sum;
						 }
						 sum = sum + div;
					 }
				 }
				 task_para.price = sum;
				 break;
			 }
		 }

		 if(task_para.price != 0)
		{
			*step = TASK_PULSE_COUNT;
		}
	 
	}
	else if(*step == TASK_PULSE_COUNT)
	{
		 str1=strstr((const char*)RxBuffer,(const char*)"pulse_count");      //?????? TASK_BUSI_TAIL
		 str2=strchr((const char*)str1,':');
		 str3=strchr((const char*)str1,',');
		 memcpy(buf,str2+1,30);

		 for(i=0;i<30;i++)
		 {
			 if(buf[i]==sss)
			 {
				 int sum = 0;
				 for(j=0;j<i;j++)
				 {
					 sum = sum*10;
					 sum = sum + (buf[j]-0x30);
				 }
				 task_para.pulse_count = sum;
				 break;
			 }
		 }
     
		 if(task_para.pulse_count != 0)
		{
			*step = TASK_PULSE_TAIL;
		}
	}
	else if(*step == TASK_PULSE_TAIL)
	{
		 str1=strstr((const char*)RxBuffer,(const char*)"pulse_tail");      //?????? TASK_BUSI_TAIL
		 str2=strchr((const char*)str1,':');
		 str3=strchr((const char*)str1,',');
		 memcpy(buf,str2+1,30);

		 for(i=0;i<30;i++)
		 {
			 if(buf[i]==0x0A)
			 {
				 int sum = 0;
				 for(j=0;j<i;j++)
				 {
					 sum = sum*10;
					 sum = sum + (buf[j]-0x30);
				 }
				 task_para.pulse_tail = sum;
				 break;
			 }
		 }
     
		 if(task_para.pulse_tail != 0)
		{
			*step = TASK_FINISH;
		}
	}
	
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
