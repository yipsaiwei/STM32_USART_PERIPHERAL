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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include  "Gpio.h"
#include  "Nvic.h"
#include  "Adc.h"
#include  "Usart.h"
#include  <stdlib.h>
#include  <string.h>
#include  <stdio.h>
#include  "retarget.h"
#include  "message_buffer.h"
#include  "event_groups.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define WAIT_AND_GET_NEW_COMMAND            (1 << 0)
#define READ_COMMAND_FROM_MASTER            (1 << 2)
#define END_OF_COMMAND                      (1 << 1)
#define RECEIVE_RESULT_FROM_SLAVE           (1 << 3)
#define RECEIVE_RESULT_FROM_MASTER          (1 << 4)
#define RECEIVE_COMMAND_FROM_INTERPRETER    (1 << 5)
#define ADC_CONVERSION_FINISHED             (1 << 6)
#define INVALID_COMMAND               0x00
#define TURN_LED_ON                   0x50
#define SUCCESSFULLY_ON               0x55
#define TURN_LED_OFF                  0x60
#define SUCCESSFULLY_OFF              0x65
#define START_ADC_CONVERSION          0x70
#define ADC_CONVERTING                0x75
#define ERROR                         0xFF
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* Definitions for master */
osThreadId_t masterHandle;
const osThreadAttr_t master_attributes = {
  .name = "master",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for slave */
osThreadId_t slaveHandle;
const osThreadAttr_t slave_attributes = {
  .name = "slave",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for commandInterpre */
osThreadId_t commandInterpreHandle;
const osThreadAttr_t commandInterpre_attributes = {
  .name = "commandInterpre",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */
MessageBufferHandle_t messageBufferFromMaster, messageBufferToMaster, messageBufferFromUsart1, messageBufferFromUsart2, messageBufferFromUsart6, messageBufferFromAdc;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void masterFunction(void *argument);
void slaveFunction(void *argument);
void commandInterpreterFunction(void *argument);

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
  rccUnresetAndEnableAdc(RCC_ADC1);
  rccUnresetAndEnableGpio(RCC_GPIOA);
  rccUnresetAndEnableGpio(RCC_GPIOB);
  rccUnresetAndEnableGpio(RCC_GPIOC);

  rccUnresetAndEnableUsart(RCC_USART6);
  rccUnresetAndEnableUsart(RCC_USART1);
  rccUnresetAndEnableUsart(RCC_USART2);

  gpioConfigurePin(gpioA, 5, GPIO_OUTPUT | GPIO_FAST_SPEED | GPIO_PUSH_PULL);  //LED

  gpioConfigurePin(gpioB, 8, GPIO_OUTPUT | GPIO_FAST_SPEED | GPIO_PUSH_PULL);// EXT. LED

  gpioConfigurePin(gpioA, 0, GPIO_ANALOG_IN | GPIO_PUSH_PULL );  //       (PA0)

  gpioConfigurePin(gpioC, 7, GPIO_ALT_FUNC | GPIO_FAST_SPEED | AF_8);  //USART6 (Rx)
  gpioConfigurePin(gpioC, 6, GPIO_ALT_FUNC | GPIO_FAST_SPEED | AF_8);  //       (Tx)

  gpioConfigurePin(gpioA, 10, GPIO_ALT_FUNC | GPIO_FAST_SPEED | AF_7);  //USART1 (Rx)
  gpioConfigurePin(gpioA, 9, GPIO_ALT_FUNC | GPIO_FAST_SPEED | AF_7);   //       (Tx)

  gpioConfigurePin(gpioA, 2, GPIO_ALT_FUNC | GPIO_FAST_SPEED | AF_7);  //USART2 (Rx)
  gpioConfigurePin(gpioA, 3, GPIO_ALT_FUNC | GPIO_FAST_SPEED | AF_7);  //       (Tx)

  usartConfiguration(usart6, USART_TX_EN | USART_RX_EN | USART_9_BIT | USART_ODD_PARITY | USART_RXNEIE_EN
                            | USART_1_STOPBIT
                            | USART_EN,
                            115200);

  usartConfiguration(usart1, USART_TX_EN | USART_RX_EN | USART_9_BIT | USART_ODD_PARITY | USART_RXNEIE_EN
                            | USART_1_STOPBIT
                            | USART_EN,
                            115200);

  usartConfiguration(usart2, USART_TX_EN | USART_RX_EN | USART_9_BIT | USART_ODD_PARITY | USART_RXNEIE_EN
                            | USART_1_STOPBIT
                            | USART_EN,
                            115200);

  int channel[] = {0};

  adcConfiguration(adc1, ADC_CONVERTER_ON | REGULAR_CHN_START_CONVERT | EOC_INT_EN
                        | SET_EOC_BIT_AFTER_REGULAR_CONVERSION_ENDED);

  adcSetChannelSequence(adc1, channel, 1);

  messageBufferFromMaster = xMessageBufferCreate(50);
  messageBufferToMaster = xMessageBufferCreate(50);
  messageBufferFromUsart1 = xMessageBufferCreate(50);
  messageBufferFromUsart2 = xMessageBufferCreate(50);
  messageBufferFromUsart6 = xMessageBufferCreate(50);
  messageBufferFromAdc = xMessageBufferCreate(50);
  RetargetInit(usart2);

  if(messageBufferFromMaster == NULL){
    printf("There was not enough heap memory space available to create the message buffer!");
  }

  if(messageBufferToMaster == NULL){
    printf("There was not enough heap memory space available to create the message buffer!");
  }

  if(messageBufferFromUsart1 == NULL){
    printf("There was not enough heap memory space available to create the message buffer!");
  }

  if(messageBufferFromUsart2 == NULL){
    printf("There was not enough heap memory space available to create the message buffer!");
  }

  if(messageBufferFromUsart6 == NULL){
    printf("There was not enough heap memory space available to create the message buffer!");
  }


  if(messageBufferFromAdc == NULL){
    printf("There was not enough heap memory space available to create the message buffer!");
  }

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of master */
  masterHandle = osThreadNew(masterFunction, NULL, &master_attributes);

  /* creation of slave */
  slaveHandle = osThreadNew(slaveFunction, NULL, &slave_attributes);

  /* creation of commandInterpre */
  commandInterpreHandle = osThreadNew(commandInterpreterFunction, NULL, &commandInterpre_attributes);

  nvicEnableIrq(ADC_IRQ);

  nvicSetPriority(ADC_IRQ, 6);

  nvicEnableIrq(USART1_IRQ);

  nvicSetPriority(USART1_IRQ, 6);

  nvicEnableIrq(USART2_IRQ);

  nvicSetPriority(USART2_IRQ, 6);

  nvicEnableIrq(USART6_IRQ);
  nvicSetPriority(USART6_IRQ, 6);


  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
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

/* USER CODE BEGIN 4 */

void  ADC_IRQHandler(void){
  uint16_t adcValue = 0;
  adcValue = adc1->DR;
  xMessageBufferSendFromISR(messageBufferFromAdc, &adcValue, 2, pdFALSE);
  }

void  USART2_IRQHandler(void){
  static  uint8_t usart2Buffer[20];
  static  int usart2BufferIndex = 0;
  usart2Buffer[usart2BufferIndex] = usart2->DR;
  if(usart2Buffer[usart2BufferIndex] == '\n'){
    usart2BufferIndex++;
    xMessageBufferSendFromISR(messageBufferFromUsart2, &usart2BufferIndex, 1, pdFALSE);
    xMessageBufferSendFromISR(messageBufferFromUsart2, usart2Buffer, usart2BufferIndex, pdFALSE);
    usart2BufferIndex = 0;
  }
  else
    usart2BufferIndex++;
}

void  USART1_IRQHandler(void){
  uint8_t commandFromMaster;
  commandFromMaster = usart1->DR;
  xMessageBufferSendFromISR(messageBufferFromUsart1, &commandFromMaster, 1, pdFALSE);
}

void  USART6_IRQHandler(void){
  static  uint8_t usart6Buffer[4];
  static  int usart6BufferIndex = 0;
  uint8_t *ptr;
  if(usart6->DR != '\0' || usart6BufferIndex != 0){
  usart6Buffer[usart6BufferIndex] = usart6->DR;
  if(usart6Buffer[usart6BufferIndex] == '\0'){
    xMessageBufferSendFromISR(messageBufferFromUsart6, usart6Buffer, 2, pdFALSE);
    if(usart6Buffer[1] > 0){
      ptr = &(usart6Buffer[2]);
      xMessageBufferSendFromISR(messageBufferFromUsart6, ptr, usart6Buffer[1], pdFALSE);
    }
    usart6BufferIndex = 0;
  }
  else
    usart6BufferIndex++;
  }
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_masterFunction */
/**
  * @brief  Function implementing the master thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_masterFunction */
void masterFunction(void *argument)
{
  /* USER CODE BEGIN 5 */
  size_t  messageSize;
  uint8_t receivedCommand, receivedData[2], resultDetail[3];
  char  commandToSend[2];
  /* Infinite loop */
  for(;;)
  {
    messageSize = xMessageBufferReceive(messageBufferToMaster, &receivedCommand, sizeof(receivedCommand), portMAX_DELAY);
    if(messageSize == 0)
      commandToSend[0] = ERROR;
    else
      commandToSend[0] = receivedCommand;
    usartTransmitWithLength(usart6, commandToSend, 1);
    xMessageBufferReceive(messageBufferFromUsart6, resultDetail, 2, portMAX_DELAY);
    if(resultDetail[1] > 0)
      xMessageBufferReceive(messageBufferFromUsart6, receivedData, resultDetail[1], portMAX_DELAY);
    messageSize =  xMessageBufferSend(messageBufferFromMaster, resultDetail, 2, portMAX_DELAY);
    if(resultDetail[1] > 0)
      messageSize =  xMessageBufferSend(messageBufferFromMaster, receivedData, resultDetail[1], portMAX_DELAY);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_slaveFunction */
/**
* @brief Function implementing the slave thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_slaveFunction */
void slaveFunction(void *argument)
{
  /* USER CODE BEGIN slaveFunction */
  uint8_t command;
  uint16_t  adcValue;
  char  resultToSend[5];

  /* Infinite loop */
  //printf("This is the slave function \n\r");
  for(;;)
  {
    xMessageBufferReceive(messageBufferFromUsart1, &command, 1, portMAX_DELAY);
    if(command == TURN_LED_ON){
      gpioWritePin(gpioA, 5, 1);
      resultToSend[0] = SUCCESSFULLY_ON;
      resultToSend[1] = 0;
    }else if (command == TURN_LED_OFF){
      gpioWritePin(gpioA, 5, 0);
      resultToSend[0] = SUCCESSFULLY_OFF;
      resultToSend[1] = 0;
    }else{
      adc1->CR[1] |= (REGULAR_CHN_START_CONVERT >> 32);
      xMessageBufferReceive(messageBufferFromAdc, &adcValue, 2, portMAX_DELAY);
      resultToSend[0] = ADC_CONVERSION_FINISHED;
      resultToSend[1] = sizeof(adcValue) + 1;
      resultToSend[2] = (adcValue & 0xFF);
      resultToSend[3] = (adcValue >> 8);
    }
    resultToSend[4] = '\0';
    usartTransmitWithLength(usart1, resultToSend, resultToSend[1] + 2);
  }
  /* USER CODE END slaveFunction */
}

/* USER CODE BEGIN Header_commandInterpreterFunction */
/**
* @brief Function implementing the commandInterpre thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_commandInterpreterFunction */
void commandInterpreterFunction(void *argument)
{
  /* USER CODE BEGIN commandInterpreterFunction */
  //char  *command;
  uint8_t command, result[2], data[3], charCommand[20], charCommandSize;
  uint16_t  adcToPrint;
  /* Infinite loop */
  //printf("Enter Command : ");
  for(;;)
  {
    xMessageBufferReceive(messageBufferFromUsart2, &charCommandSize, 1, portMAX_DELAY);
    xMessageBufferReceive(messageBufferFromUsart2, charCommand, charCommandSize, portMAX_DELAY);
      if(strncasecmp((char  *)charCommand, "TurnOnLed\n", charCommandSize) == 0)
        command = TURN_LED_ON;
      else if(strncasecmp((char  *)charCommand, "TurnOffLed\n", charCommandSize) == 0)
        command = TURN_LED_OFF;
      else if(strncasecmp((char  *)charCommand, "StartADC\n", charCommandSize) == 0)
        command = START_ADC_CONVERSION;
      else
        command = INVALID_COMMAND;

      if(command == INVALID_COMMAND)
        printf("Invalid Command! Please try again.\n");
      else{
        xMessageBufferSend(messageBufferToMaster, &command, sizeof(command), portMAX_DELAY);
      }

      if(command != INVALID_COMMAND){
         xMessageBufferReceive(messageBufferFromMaster, result, 2, portMAX_DELAY);
         if(result[1] > 0)
           xMessageBufferReceive(messageBufferFromMaster, data, result[1], portMAX_DELAY);
        if(result[0] == SUCCESSFULLY_ON)
          printf("LED Switched ON\n\r");
        else if(result[0] == SUCCESSFULLY_OFF)
          printf("LED Switched OFF\n\r");
        else{
          adcToPrint = data[0] | (data[1] << 8);
          printf("ADC value : %d\n\r", adcToPrint);
        }

  }
  /* USER CODE END commandInterpreterFunction */
  }
}

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM9 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM9) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
