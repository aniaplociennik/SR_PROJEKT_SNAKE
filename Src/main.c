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
#include "tim.h"
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
volatile int flag; //Domyslnie na 0 ustawiona
uint8_t dir_flag = none; //zmienna przechowujaca kierunek poruszania
// uint8_t dir_flag; //zmienna przechowujaca kierunek
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void initPlay(void)
{		ssd1331_init();
    	ssd1331_clear_screen(GREEN_BACKGROUND);
    	ssd1331_display_string(20, 0, (const uint8_t*)"Sssnake", FONT_1608, GREEN);
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
        ssd1331_display_string(20, 39, (const uint8_t*)"Click blue", FONT_1206, GREEN);
        ssd1331_display_string(5, 51, (const uint8_t*)"button to play", FONT_1206, GREEN);
}
//Funkcja sprawdza czy nastapila kolizja -> jesli tak wyswietla komunikat na ekranie i przerywa petle while
bool IsGameOver()
{
	//Sprawdza funkcje odpowiadajaca za kolizje weza, funkcja zwraca "true" jesli nastapilo zderzenie weza
	if(checkCollision()){
		ssd1331_clear_screen(GREEN_BACKGROUND);
		ssd1331_display_string(20, 10, (const uint8_t*)"GAME OVER!", FONT_1206, WHITE);
		ssd1331_display_string(5, 20, (const uint8_t*)"You received", FONT_1206, WHITE);
		ssd1331_display_string(30, 30, (const uint8_t*)"points!", FONT_1206, WHITE);
		int points=Points();
		ssd1331_display_num(70, 20, points, sizeof(int), FONT_1206, WHITE);

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
		printf("START! \nMove joystick.\r\n");
	}
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int _write(int file, char *ptr, int len)
{
	int i;
	for(i=0; i<len; i++)
	{
		ITM_SendChar(*ptr++);
	}
	return len;
}
//Callback do obslugi przerwania od timera10
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim == &htim10){
		flag = 1;
	}
}
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
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */
  //Uruchomienie przetwornika w trybie DMA
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)Joystick, 2);//Rzutowanie na 32-bitowego inta, aby nie było warninga
  HAL_TIM_Base_Start_IT(&htim10);
  initPlay(); //Inicjalizacja ekranu startowego
  printf("Push blue button to start!. \r\n");
  //dir_flag = none;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

 while (!IsGameOver())
 {
	  if(Joystick[0]>Thresholdup && dir_flag!=down){//Jesli joystick do gory i nie w dol
		  dir_flag=up;
	  }
	  else if(Joystick[0]<Thresholddown && dir_flag!=up){//Joystic wychylony w dol i nie w gore
		  dir_flag=down;
	  }
	  else if(Joystick[1]>Thresholdup && dir_flag!=left){//Joystic wychylony w prawo i nie w lewo
		  dir_flag=right;
	  }
	  else if(Joystick[1]<Thresholddown && dir_flag!=right){//Joystic wychylony w lewo i nie w prawo
		  dir_flag=left;
	  }
	  //Jesli timer sie przeterminowal (czestotliwosc 5HZ)
	  if(flag == 1){
		  flag=0;
		  if(dir_flag != none)
			  MoveSnake(dir_flag);
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  printf("You've lost! GAME OVER!!!\r\n");
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
