/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for InputTask */
osThreadId_t InputTaskHandle;
const osThreadAttr_t InputTask_attributes = {
  .name = "InputTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for SensorTask */
osThreadId_t SensorTaskHandle;
const osThreadAttr_t SensorTask_attributes = {
  .name = "SensorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for ScreenTask */
osThreadId_t ScreenTaskHandle;
const osThreadAttr_t ScreenTask_attributes = {
  .name = "ScreenTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for CommTask */
osThreadId_t CommTaskHandle;
const osThreadAttr_t CommTask_attributes = {
  .name = "CommTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for sensorQueueScreen */
osMessageQueueId_t sensorQueueScreenHandle;
const osMessageQueueAttr_t sensorQueueScreen_attributes = {
  .name = "sensorQueueScreen"
};
/* Definitions for sensorQueueComm */
osMessageQueueId_t sensorQueueCommHandle;
const osMessageQueueAttr_t sensorQueueComm_attributes = {
  .name = "sensorQueueComm"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartInputTask(void *argument);
extern void StartSensorTask(void *argument);
extern void StartScreenTask(void *argument);
extern void StartCommTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of sensorQueueScreen */
  sensorQueueScreenHandle = osMessageQueueNew (4, 12, &sensorQueueScreen_attributes);

  /* creation of sensorQueueComm */
  sensorQueueCommHandle = osMessageQueueNew (10, 12, &sensorQueueComm_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of InputTask */
  InputTaskHandle = osThreadNew(StartInputTask, NULL, &InputTask_attributes);

  /* creation of SensorTask */
  SensorTaskHandle = osThreadNew(StartSensorTask, NULL, &SensorTask_attributes);

  /* creation of ScreenTask */
  ScreenTaskHandle = osThreadNew(StartScreenTask, NULL, &ScreenTask_attributes);

  /* creation of CommTask */
  CommTaskHandle = osThreadNew(StartCommTask, NULL, &CommTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartInputTask */
/**
  * @brief  Function implementing the InputTask thread.
  * @param  argument: Not used
  * @retval None
  */    	
/* USER CODE END Header_StartInputTask */
__weak void StartInputTask(void *argument)
{
  /* USER CODE BEGIN StartInputTask */
  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_7); 
    osDelay(500);
  }
  /* USER CODE END StartInputTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

