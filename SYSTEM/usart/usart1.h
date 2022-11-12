/*
 * @Description: 串口1的头文件
 * @Author: TOTHTOT
 * @Date: 2022-11-12 01:09:18
 * @LastEditTime: 2022-11-12 01:18:16
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Learn\stm32\CubeMX_Project\STM32F103C8T6\STM32F103C8T6(HAL+FreeRTOS)\HARDWARE\USART\usart1.h
 */
#ifndef __USART1_H
#define __USART1_H

#define USART_REC_LEN 200
#define USART_SEND_LEN 200
#define EN_USART1_RX 1
#define RXBUFFERSIZE 1

extern uint8_t USART_RX_BUF[USART_REC_LEN];
extern uint16_t USART_RX_STA;
extern uint8_t aRxBuffer[RXBUFFERSIZE];

void USER_UART_IRQHandler(UART_HandleTypeDef *huart);
void u1_printf(char *format, ...);




#endif
