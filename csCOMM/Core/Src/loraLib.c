#include "main.h"
#include "stm32f4xx_hal.h"
#include "loraLib.h"

SensorData sensorData;
extern UART_HandleTypeDef huart6;
////////////////////////////////////////////////
// HANGI KANALI ISTIYORSAN ONU YORUMDAN CIKAR //
////////////////////////////////////////////////

//#define LORA_CHANNEL   0x17  // 433.125 MHz
//#define LORA_CHANNEL   0x18  // 434.125 MHz
//#define LORA_CHANNEL   0x19  // 435.125 MHz
//#define LORA_CHANNEL   0x1A  // 436.125 MHz
#define LORA_CHANNEL   0x1B  // 437.125 MHz
//#define LORA_CHANNEL   0x1C  // 438.125 MHz
//#define LORA_CHANNEL   0x1D  // 439.125 MHz
//#define LORA_CHANNEL   0x1E  // 440.125 MHz

////////////YENI KANAL EKLEMEK ICIN BURAYA BAK//////////
// 17 = 433.15 MHz 1B = 437.15 MHz
// taban frekans 410.15 decimal değeri hexe çevirip ekle
// mesela 437 - 410 = 27 nin hex değeri 1B

unsigned char loraCheckSumCalc(void *data, size_t size){
    int checkSum = 0;

    uint8_t buffer[size];
    memcpy(buffer, data, size);

    for(int i = 0; i < size; i++){
    	checkSum += buffer[i];
    }

    return (unsigned char) (checkSum % 256);
}

void LoraConfigure(void) {
    uint8_t config[] = {
        0xC0,
		0x00, 0x08,
        0x00, 0x07,
		0x64, 0x00, LORA_CHANNEL, 0x43, 0x00, 0x00
    };

    uint8_t response[6];

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, 1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 1);
    HAL_Delay(100);

    HAL_UART_Transmit(&huart6, config, sizeof(config), HAL_MAX_DELAY);
    HAL_Delay(100);  // Modül cevap için zaman ister

    HAL_UART_Receive(&huart6, response, sizeof(response), 500);

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, 0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);
    HAL_Delay(100);
}

void LoraSendData(UART_HandleTypeDef *huart, uint8_t addrH, uint8_t addrL, uint8_t channel, void *data, size_t size) {
    uint8_t fixedModeInfo[3] = {addrH, addrL, channel};
	uint8_t bufferSize = size + 6;

    uint8_t buffer[bufferSize];
    buffer[3] = 0xAA;
    buffer[bufferSize - 1] = 0x0D;
    memcpy(buffer, fixedModeInfo, 3);
    memcpy(buffer + 4, data, size);
    buffer[bufferSize - 2] = loraCheckSumCalc(&buffer[3], bufferSize - 5);

    HAL_UART_Transmit(huart, buffer, bufferSize, HAL_MAX_DELAY);
}
