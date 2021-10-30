/*
 * app_US100.c
 *
 *  Created on: Oct 29, 2021
 *      Author: stephensheldon
 */

#include "app_US100.h"
#include <stdlib.h>


#define TRUE 1
#define FALSE 0

static UART_HandleTypeDef *huart;

// Distance is always positive, max(255 * 256 + 255) = 65535
uint16_t distance = 0;

// If values are out of range then distance returned will either be 0 or UINT16_MAX
uint16_t min_mm_distance = 0;
uint16_t max_mm_distance = UINT16_MAX;

// Indicates if a value has been received from US-100. Cleared once read.
uint8_t distance_avail = FALSE;

// Indicates if a request has been sent to US-100 and neither a response has been received or timed out.
uint8_t distance_pending = FALSE;

// INdicates if a request was made while an existing operation is in progress.
uint8_t distance_request_queued = FALSE;

/**
 *
 * @param huartInit
 */
void us100_init(UART_HandleTypeDef *huartInit) {
	huart = huartInit;
}


//uint8_t us100_write(uint8_t val, int size) {
//
//	char buffer[size];
//	itoa(i, buffer, size);
//	HAL_UART_Transmit(huart, buffer, size, MAX_HAL_DELAY);
//}
//
//void UART_SEND(UART_HandleTypeDef *huart, char buffer[])
//{
//HAL_UART_Transmit(huart, (uint8_t*) buffer, strlen(buffer), HAL_MAX_DELAY);
//}
//
//void UART_SEND_INT(UART_HandleTypeDef *huart, int i, int m)
//{
//	char buffer[2];
//	itoa(i, buffer, 2);
////	HAL_UART_Transmit(huart, (uint8_t*) buffer, strlen(buffer), HAL_MAX_DELAY);
//	HAL_UART_Transmit(huart, (uint8_t*)'h', 1, HAL_MAX_DELAY);
//
//	if(m == 1)
//		HAL_UART_Transmit(huart, (uint8_t*)"\n\r", 2, HAL_MAX_DELAY);
//}
