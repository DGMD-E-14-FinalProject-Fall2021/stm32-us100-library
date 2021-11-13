/*
 * app_US100.c
 *
 *  Created on: Oct 29, 2021
 *  Author: stephensheldon
 */

#include "app_US100.h"
#include <stdlib.h>


// Because I can.
#define TRUE (1 == 1)
#define FALSE (!TRUE)

// Sensor can be in several states.
// IDLE - Waiting to send next cmd to sensor to get distance
// WAIT_DIST - A request for distance has been sent to the sensor but not received yet.
// CALC_DIST - A distance value has been received from the sensor.
enum { IDLE, WAIT_DIST, CALC_DIST };

char state = IDLE;                 // Initial starting state is IDLE
char try = 0;                      // Utilized in case the sensor times out
uint16_t distance = 0;             // Distance is always positive, max(255 * 256 + 255) = 65535
uint8_t cmd_dist = 0x55;           // 0x55 is value to send to sensor to request distance
uint8_t UART5_rx_buffer[2] = {0};  // Buffer to receive distance data from sensor

static UART_HandleTypeDef *huart;
static DMA_HandleTypeDef *hdma_uart_rx;

// If values are out of range then distance returned will either be 0 or UINT16_MAX
uint16_t min_mm_distance = 0;
uint16_t max_mm_distance = UINT16_MAX;

/**
 * Initializes the US-100 UART and DMA.
 * @param huart_init        The UART_HandleTypeDef associated with the UART connected to sensor.
 * @param hdma_uart_rx_init The DMA_HandleTypeDef associated with the DMA to be used with sensor.
 */
void us100_init(UART_HandleTypeDef *huart_init, DMA_HandleTypeDef *hdma_uart_rx_init) {
	huart = huart_init;
	hdma_uart_rx = hdma_uart_rx_init;
}

/**
 * Writes a value to the sensor.
 * @param val Value to write to sensor.
 */
void us100_write(uint8_t val) {
	HAL_UART_Transmit(huart, &val, 1, HAL_MAX_DELAY);
}

/**
 * Read value from the sensor using DMA.
 * @param buffer Buffer to store data received from sensor.
 * @param size   Size of buffer.
 */
void us100_read(uint8_t *buffer, uint8_t size) {
	HAL_UART_Receive_DMA(huart, buffer, size);
}

/**
 * Function to check if data is available from the US-100 sensor.
 * @return TRUE if data is available and FALSE otherwise.
 */
uint8_t us100_data_available(void) {
	if (state == IDLE)
	{
		// Send request to measure distance
		us100_write(cmd_dist);
		us100_read(UART5_rx_buffer, 2);
		// Change state to wait for response
		state = WAIT_DIST;
		try = 0;
	}
	else if (state == CALC_DIST)
	{
		// Calculate the distance
		distance = (UART5_rx_buffer[0] << 8) + UART5_rx_buffer[1];
		state = IDLE;
		return TRUE;
	}

  HAL_Delay(100);

  // Retry after 5 seconds
  if (++try >= 50) {
  	state = IDLE;
  }
  return FALSE;
}

/**
 * Returns the distance calculated by the US-100 sensor.
 * @return Distance calculated by sensor in millimeters.
 */
uint16_t us100_get_distance(void) {

	distance = (UART5_rx_buffer[0] << 8) + UART5_rx_buffer[1];

	if (distance > max_mm_distance)
	{
		distance = UINT16_MAX;
	}
	else if (distance < min_mm_distance) {
		distance = 0;
	}
	return distance;
}

/**
 * Callback function for DMA.
 * @param huart The UART_HandleTypeDef that is utilized for US-100 sensor UART channel.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (state == WAIT_DIST) {
		state = CALC_DIST;
	}
}

