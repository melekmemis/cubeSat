/*
 * w25q32.h
 *
 *  Created on: Jun 8, 2026
 *      Author: memis
 */

#ifndef INC_W25Q32_H_
#define INC_W25Q32_H_

#include "main.h"

extern SPI_HandleTypeDef hspi1;

#define W25_CS_PORT GPIOA
#define W25_CS_PIN  GPIO_PIN_4

#define CS_LOW()  HAL_GPIO_WritePin(W25_CS_PORT, W25_CS_PIN, GPIO_PIN_RESET)
#define CS_HIGH() HAL_GPIO_WritePin(W25_CS_PORT, W25_CS_PIN, GPIO_PIN_SET)

uint8_t W25Q_Init(void);

uint32_t W25Q_ReadID();

void W25Q_Read(uint32_t addr, uint8_t *data, uint16_t len);

void W25Q_WritePage(uint32_t addr, uint8_t *data, uint16_t len);

void W25Q_EraseSector(uint32_t addr);

void W25Q_Reset(void);

#endif
