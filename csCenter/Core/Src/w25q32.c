/*
 * w25q32.c
 *
 *  Created on: Jun 8, 2026
 *      Author: memis
 */

#include "w25q32.h"

#define CMD_JEDEC_ID       0x9F
#define CMD_WRITE_ENABLE   0x06
#define CMD_READ_STATUS1   0x05
#define CMD_READ_DATA      0x03
#define CMD_PAGE_PROGRAM   0x02
#define CMD_SECTOR_ERASE   0x20

static uint8_t W25Q_ReadStatus(void)
{
    uint8_t cmd = CMD_READ_STATUS1;
    uint8_t status = 0;

    CS_LOW();

    HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);
    HAL_SPI_Receive(&hspi1, &status, 1, 100);

    CS_HIGH();

    return status;
}

static void W25Q_WaitBusy(void)
{
    while(W25Q_ReadStatus() & 0x01)
    {
        HAL_Delay(1);
    }
}

static void W25Q_WriteEnable(void)
{
    uint8_t cmd = CMD_WRITE_ENABLE;

    CS_LOW();

    HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);

    CS_HIGH();
}

uint8_t id[3];

uint8_t W25Q_Init(void)
{


    if(id[0] == 0xEF)
    {
        return 1;
    }

    return 0;
}

uint32_t W25Q_ReadID()
{
	uint8_t tx[4] = {0x9F, 0x00, 0x00, 0x00};
	uint8_t rx[4] = {0};

    CS_LOW();

    HAL_SPI_Transmit(&hspi1, tx, 1, 1000);
    HAL_SPI_Receive(&hspi1, rx, 3, 3000);

    CS_HIGH();

    id[1] = rx[1];
    id[2] = rx[2];
    id[3] = rx[3];
    return ((id[1] << 16) | (id[2] << 8) | id[3]);
}

void W25Q_Reset(void) {
	uint8_t tData[2];
	tData[0] = 0x66;
	tData[1] = 0x99;

	CS_LOW();
	HAL_SPI_Transmit(&hspi1, tData, 2, 1000);
	CS_HIGH();

	HAL_Delay(100);
}

void W25Q_Read(uint32_t addr, uint8_t *data, uint16_t len)
{
    uint8_t cmd[4];

    cmd[0] = CMD_READ_DATA;
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;

    CS_LOW();

    HAL_SPI_Transmit(&hspi1, cmd, 4, 100);
    HAL_SPI_Receive(&hspi1, data, len, 1000);

    CS_HIGH();
}

void W25Q_WritePage(uint32_t addr, uint8_t *data, uint16_t len)
{
    uint8_t cmd[4];

    W25Q_WriteEnable();

    cmd[0] = CMD_PAGE_PROGRAM;
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;

    CS_LOW();

    HAL_SPI_Transmit(&hspi1, cmd, 4, 100);
    HAL_SPI_Transmit(&hspi1, data, len, 1000);

    CS_HIGH();

    W25Q_WaitBusy();
}

void W25Q_EraseSector(uint32_t addr)
{
    uint8_t cmd[4];

    W25Q_WriteEnable();

    cmd[0] = CMD_SECTOR_ERASE;
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;

    CS_LOW();

    HAL_SPI_Transmit(&hspi1, cmd, 4, 100);

    CS_HIGH();

    W25Q_WaitBusy();
}
