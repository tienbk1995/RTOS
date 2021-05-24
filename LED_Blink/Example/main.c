/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
volatile uint8_t blink = 0;
TIM_HandleTypeDef timer6;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void LedBlink(void);
uint8_t GPIO_Init_User(void);
void SoftDelay(void);
void Timer_Init(TIM_HandleTypeDef *tim);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  // MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // while (1)
  // {
    /* USER CODE END WHILE */
		//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
    //LedBlink();
  GPIO_Init_User();
  Timer_Init(&timer6);
  HAL_TIM_Base_Start_IT(&timer6);
    /* USER CODE BEGIN 3 */
  // }
  /* USER CODE END 3 */
  while (1);
}

/**
 * @brief Configuration for PD15 - LED, PC13 - USER BUTTON
 * @retval None
 */
uint8_t GPIO_Init_User(void)
{
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* PA0 - User Button */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* PB2 - Interrupt Triggered*/
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /* PD13 - Led Blinking */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  /* Reset PD13 - Led */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
  /* Set Priority and Enable Interrupt for EXTI0_IRQn */
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0x2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
  /* Set Priority and Enable Interrupt for EXTI2_IRQn */
  HAL_NVIC_SetPriority(EXTI2_IRQn, 0x0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
  return 0;
}

/**
 * @brief User Button Interrupt Handling (EXTI0_IRQ)
 * @retval None
 */
void EXTI0_IRQHandler(void)
{
  // /* Start Timer */
  // HAL_TIM_Base_Start_IT(&timer6);
  /* IRQ Handler */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  // HAL_TIM_Base_Start_IT(&timer6);
}

/**
 * @brief Interrupt Handling (EXTI2_IRQ)
 * @retval None
 */
void EXTI2_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

/**
 * @brief Callback function
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_0)
  {
    blink = 1;
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
    // SoftDelay();
    while(blink);
  }
  else
  {
    blink = 0;
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
  }
}
/**
 * @brief Software Delay Configuration
 * @retval None
 */
void SoftDelay(void)
{
  uint32_t count = 10;
  while(count > 0)
  {
    count -= 1;
    HAL_Delay(500);
  }
  /* Triggering EXTI line 2 */
  __HAL_GPIO_EXTI_GENERATE_SWIT(GPIO_PIN_2);
}

/**
 * @brief Led Blinking
 * @retval None
 */
void LedBlink(void)
{
  uint32_t LED = GPIO_PIN_12;
  GPIOD->BSRR |= 0x0000F000;
  GPIOD->BSRR = (uint32_t)LED << 16;
  HAL_Delay(200);
  for (int i = 0; i < 3; i++)
  {
    GPIOD->BSRR |= 0x0000F000;
    LED <<= 1;
    HAL_Delay(200);
    GPIOD->BSRR = (uint32_t)LED << 16;
    HAL_Delay(200);
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

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

/**
 * @brief Timer Initialize
 * @retval None
 */
void Timer_Init(TIM_HandleTypeDef *tim)
{
  tim->Instance = TIM6;
  tim->Init.Prescaler = 47999; // 48Mhz/48000 = 1000Mhz
  tim->Init.Period = 4999; // 1000Mhz/5000 = 0.2Hz = 5s
  __HAL_RCC_TIM6_CLK_ENABLE(); // Enable clock for Timer 6
  /* Set Priority and Enable Interrupt for TIM6_IRQn */
  HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0x1, 0);
  HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
  /* Init Timer */
  HAL_TIM_Base_Init(tim);
  // HAL_TIM_Base_Start_IT(tim);
}

/**
 * @brief Timer Handler
 * @retval None
 */
void TIM6_DAC_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&timer6);
}

/**
 * @brief Timer Callback Function
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM6)
  {
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
  // HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
  HAL_Delay(1000);
  /* Stop Timer */
  HAL_TIM_Base_Stop_IT(&timer6);
      /* Triggering EXTI line 2 */
  __HAL_GPIO_EXTI_GENERATE_SWIT(GPIO_PIN_2);
    // HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
    // HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
  }
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
