/*
 * app_US100.c
 *
 *  Created on: Oct 29, 2021
 *      Author: stephensheldon
 */

#include "app_US100.h"
#include <stdlib.h>



/*
 * Returned by data_available() to indicate which values have been requested previously and are now available
 */
#define DISTANCE    0x1

/*
 * Fancy
 */
#define TRUE (1 == 1)
#define FALSE (!TRUE)

static UART_HandleTypeDef *huart;

static DMA_HandleTypeDef *hdma_uart_rx;


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


void us100_init(UART_HandleTypeDef *huart_init, DMA_HandleTypeDef *hdma_uart_rx_init) {
	huart = huart_init;
	hdma_uart_rx = hdma_uart_rx_init;
}


void us100_write(uint8_t val) {
	HAL_UART_Transmit(huart, &val, 1, HAL_MAX_DELAY);
}

void us100_read(uint8_t *buffer, uint8_t size) {
	HAL_UART_Receive_DMA(huart, buffer, size);
}



