/*
 * app_US100.h
 *
 *  Created on: Oct 29, 2021
 *      Author: stephensheldon
 */

#ifndef APP_US100_H_
#define APP_US100_H_

#include "stm32l4xx_hal.h"


void us100_init(UART_HandleTypeDef *huart_init, DMA_HandleTypeDef *hdma_uart_rx_init);

void us100_write(uint8_t val);

void us100_read(uint8_t *buffer, uint8_t size);

#endif /* APP_US100_H_ */
