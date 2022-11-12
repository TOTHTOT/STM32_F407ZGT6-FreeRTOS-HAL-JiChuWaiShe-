/*
 * @Description: 
 * @Author: TOTHTOT
 * @Date: 2022-07-15 11:08:54
 * @LastEditTime: 2022-07-15 13:47:44
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\Learn\stm32\BASIC_PROJECT\STM32_F407ZGT6(FreeRTOS+HAL)\HARDWARE\LED\led.h
 */
#ifndef __LED_H
#define __LED_H

#include "gpio.h"

#define LED0_TOGGLE HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9)
#define LED1_TOGGLE HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_10)

#define LED0_OFF HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET)
#define LED0_ON HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET)

#define LED1_OFF HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET)
#define LED1_ON HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET)

#endif



