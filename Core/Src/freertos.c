/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
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
#include "led.h"
#include "key.h"
#include "usart.h"
#include "string.h"
#include "delay.h"
#include "usart1.h"
// #include "event_groups.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
EventGroupHandle_t Key_EventGroupHandle;
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
osThreadId defaultTaskHandle;
osThreadId LED_TASKHandle;
osThreadId KEY_TASKHandle;
osThreadId USART1_TASKHandle;
osSemaphoreId Usart1_Receive_BinSemaphoreHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const *argument);
void led_task(void const *argument);
void key_task(void const *argument);
void usart1_task(void const *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize);

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationIdleHook(void);
void vApplicationTickHook(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{
}

__weak unsigned long getRunTimeCounterValue(void)
{
    return 0;
}
/* USER CODE END 1 */

/* USER CODE BEGIN 2 */
__weak void vApplicationIdleHook(void)
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
    task. It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()). If the application makes use of the
    vTaskDelete() API function (as this demo application does) then it is also
    important that vApplicationIdleHook() is permitted to return to its calling
    function, because it is the responsibility of the idle task to clean up
    memory allocated by the kernel to any task that has since been deleted. */
}
/* USER CODE END 2 */

/* USER CODE BEGIN 3 */
__weak void vApplicationTickHook(void)
{
    /* This function will be called by each tick interrupt if
    configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
    added here, but the tick hook is called from an interrupt context, so
    code must not attempt to block, and only the interrupt safe FreeRTOS API
    functions can be used (those that end in FromISR()). */
}
/* USER CODE END 3 */

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
    *ppxTimerTaskStackBuffer = &xTimerStack[0];
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
    /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* Create the semaphores(s) */
    /* definition and creation of Usart1_Receive_BinSemaphore */
    osSemaphoreDef(Usart1_Receive_BinSemaphore);
    Usart1_Receive_BinSemaphoreHandle = osSemaphoreCreate(osSemaphore(Usart1_Receive_BinSemaphore), 1);

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    xSemaphoreTake(Usart1_Receive_BinSemaphoreHandle, portMAX_DELAY); // read semaphore
                                                                      /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* definition and creation of defaultTask */
    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /* definition and creation of LED_TASK */
    osThreadDef(LED_TASK, led_task, osPriorityNormal, 0, 128);
    LED_TASKHandle = osThreadCreate(osThread(LED_TASK), NULL);

    /* definition and creation of KEY_TASK */
    osThreadDef(KEY_TASK, key_task, osPriorityAboveNormal, 0, 128);
    KEY_TASKHandle = osThreadCreate(osThread(KEY_TASK), NULL);

    /* definition and creation of USART1_TASK */
    osThreadDef(USART1_TASK, usart1_task, osPriorityHigh, 0, 256);
    USART1_TASKHandle = osThreadCreate(osThread(USART1_TASK), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    // Create event flag group
    Key_EventGroupHandle = xEventGroupCreate();
    /* USER CODE END RTOS_THREADS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument)
{
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for (;;)
    {
        osDelay(1);
    }
    /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_led_task */
/**
 * @brief Function implementing the LED_TASK thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_led_task */
void led_task(void const *argument)
{
    /* USER CODE BEGIN led_task */
    /* Infinite loop */
    for (;;)
    {
        LED0_TOGGLE;
        // osDelay(500);
        delay_ms(500);
    }
    /* USER CODE END led_task */
}

/* USER CODE BEGIN Header_key_task */
/**
 * @brief Function implementing the KEY_TASK thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_key_task */
void key_task(void const *argument)
{
    /* USER CODE BEGIN key_task */
    /* Infinite loop */
    EventBits_t key_eventvalue;
    char str[] = "my name is yyh\r\n";
    for (;;)
    {
        key_eventvalue = xEventGroupWaitBits(Key_EventGroupHandle, 1 << 0 | 1 << 1, pdTRUE, pdFALSE, portMAX_DELAY);
        if (key_eventvalue != 0)
        {
            switch (key_eventvalue)
            {
            case 1 << 0:
                // HAL_UART_Transmit(&huart1, str, sizeof(str), );
                // HAL_UART_Transmit_DMA(&huart1, str, sizeof(str));
                u1_printf("%s\r\n", str);
                // printf("%s\r\n", str);
                LED1_TOGGLE;
                break;
            case 1 << 1:
                printf("wk_up\r\n");
                break;
            default:
                break;
            }
        }
        LED1_TOGGLE;
        // osDelay(40);
        delay_ms(40);
    }
    /* USER CODE END key_task */
}

/* USER CODE BEGIN Header_usart1_task */
/**
 * @brief Function implementing the USART1_TASK thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_usart1_task */
void usart1_task(void const *argument)
{
    /* USER CODE BEGIN usart1_task */
    /* Infinite loop */
    uint8_t len = 0;
    extern DMA_HandleTypeDef hdma_usart1_rx;
    for (;;)
    {
        xSemaphoreTake(Usart1_Receive_BinSemaphoreHandle, portMAX_DELAY);
        // len = USART_REC_LEN - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx); // get lenth of the data
        printf("data:%s len:%d\r\n", USART_RX_BUF, len);
        memset(USART_RX_BUF, '\0', sizeof(USART_RX_BUF)); // initialization buf
        USART_RX_STA = 0;
        delay_ms(1);
    }
    /* USER CODE END usart1_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
