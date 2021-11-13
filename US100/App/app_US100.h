/*
 * app_US100.h
 *
 *  Created on: Oct 29, 2021
 *  Author: stephensheldon
 */

#ifndef APP_US100_H_
#define APP_US100_H_

#include "stm32l4xx_hal.h"


void us100_init(UART_HandleTypeDef *huart_init, DMA_HandleTypeDef *hdma_uart_rx_init);
uint8_t us100_data_available(void);
uint16_t us100_get_distance(void);


#endif /* APP_US100_H_ */
