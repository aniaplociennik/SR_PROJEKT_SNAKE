/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SSD1331.h" //biblioteka do wyswietlacza OLED
#include <stdio.h>
#include <stdlib.h>
#include "snake.h"
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
uint16_t Joystick[2];//Joystick[0]->Os Y, Joystick[1]->Os X
const uint16_t Thresholdup = 4000;//Gorny prog przetwornika
const uint8_t Thresholddown = 100;//Dolny prog przetwornika
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void initPlay(void)
{		ssd1331_init();
    	ssd1331_clear_screen(GREEN_BACKGROUND);
    	ssd1331_display_string(20, 0, "Sssnake", FONT_1608, GREEN);
    	ssd1331_draw_circle(37,23,SNAKE_RAD, GREEN_SNAKE);
    	ssd1331_draw_circle(41,23,SNAKE_RAD, GREEN_SNAKE);
    	ssd1331_draw_circle(45,23,SNAKE_RAD, GREEN_SNAKE);
    	ssd1331_draw_circle(49,23,SNAKE_RAD, GREEN_SNAKE);
    	ssd1331_draw_circle(53,23,SNAKE_RAD, GREEN_SNAKE);
    	ssd1331_draw_circle(57,23,SNAKE_RAD, GREEN_SNAKE);
    	ssd1331_draw_circle(57,27,SNAKE_RAD, GREEN_SNAKE);
    	ssd1331_draw_circle(57,31,SNAKE_RAD, GREEN_SNAKE);
    	ssd1331_draw_circle(57,35,SNAKE_RAD, GREEN_SNAKE);
    	ssd1331_draw_circle(53,35,SNAKE_RAD, GREEN_SNAKE);
    	ssd1331_draw_circle(49,35,SNAKE_RAD, YELLOW);
    	ssd1331_draw_circle(37,35,SNAKE_RAD, RED);
        ssd1331_display_string(20, 39, "Click blue", FONT_1206, GREEN);
        ssd1331_display_string(5, 51, "button to play", FONT_1206, GREEN);
}
//Funkcja sprawdza czy nastapila kolizja -> jesli tak wyswietla komunikat na ekranie i przerywa petle while
bool IsGameOver()
{
	//Sprawdza funkcje odpowiadajaca za kolizje weza, funkcja zwraca "true" jesli nastapilo zderzenie weza
	if(checkCollision()){
		ssd1331_clear_screen(BLACK);
		ssd1331_display_string(20, 10, "GAME OVER!!!", FONT_1206, GREEN);
		return true;
	}
	else
		return false;
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == Button_Pin)
	{
		ssd1331_clear_screen(GREEN_BACKGROUND);
		initSnake();
	}
}

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
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  //Uruchomienie przetwornika w trybie DMA
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)Joystick, 2);//Rzutowanie na 32-bitowego inta, aby nie było warninga
  initPlay(); //Inicjalizacja ekranu startowego
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (!IsGameOver())
  {
	  //Jesli joystick do gory
	  if(Joystick[0]>Thresholdup)
	  {
		  for(;;)
	  	  {
	  	  	  MoveSnake(up);//jedz do gory
	  	  	  HAL_Delay(200);
	  	  	  if(Joystick[0]<Thresholddown||Joystick[1]>Thresholdup||Joystick[1]<Thresholddown) break;
	  	  	  if(IsGameOver()) break;
	  	  }
	  }//Joystic wychylony w dol
	  else if(Joystick[0]<Thresholddown)
	  {
		  for(;;)
		  {
			  MoveSnake(down);//zawroc w dol
			  HAL_Delay(200);
			  if(Joystick[0]>Thresholdup||Joystick[1]>Thresholdup||Joystick[1]<Thresholddown) break;
			  if(IsGameOver()) break;
		  }
	  }//Joystic wychylony w prawo
	  else if(Joystick[1]>Thresholdup)
	  {
		  for(;;)
		  {
	  	  	  MoveSnake(right);//skrec w prawo
	  	  	  HAL_Delay(200);
	  	  	  if(Joystick[0]>Thresholdup||Joystick[0]<Thresholddown||Joystick[1]<Thresholddown) break;
	  	  	  if(IsGameOver()) break;
		  }
	  }//Joystic wychylony w lewo
	  else if(Joystick[1]<Thresholddown)
	  {
		  for(;;)
		  {
			  MoveSnake(left);//skrec w lewo
			  HAL_Delay(200);
			  if(Joystick[0]>Thresholdup||Joystick[0]<Thresholddown||Joystick[1]>Thresholdup) break;
			  if(IsGameOver()) break;
		  }
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
