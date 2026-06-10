/*
 * sdCard.c
 *
 *  Created on: Aug 23, 2025
 *      Author: memis
 */

#include "sdCard.h"
#include "fatfs.h"
#include "main.h"
#include "stdio.h"
#include "string.h"

FATFS FatFs;
FIL fil;
FRESULT fres;

extern SensorData sensorData;

int dataSize;
int yazdi;

void sdCardOpen() {
	fres = f_mount(&FatFs, "", 1);
	if (fres != FR_OK) { }
}

void sdCardClose() {
	f_mount(NULL, "", 0);
}

void sdCardWrite() {
	fres = f_open(&fil, "yenid.csv", FA_WRITE | FA_OPEN_APPEND);
	f_lseek(&fil, f_size(&fil));
	if(fres == FR_OK) {
	} else {
	}
	char str[200];
	sprintf(str, "Time: %d:%d:%d, accX: %f, accY: %f, accZ: %f, pitch: %f, roll: %f, yaw: %f, Altitude: %f, SensorC BatV: %f, SensorC BatC: %f\r\n",
		    	             sensorData.rtcHour, sensorData.rtcMinute, sensorData.rtcSecond, sensorData.accX, sensorData.accY, sensorData.accZ, sensorData.pitch, sensorData.roll, sensorData.yaw, sensorData.altitude, sensorData.busVoltageFloat, sensorData.currentFloat);

	dataSize = (int)strlen(str);

	UINT bytesWrote;
	fres = f_write(&fil, str, dataSize, &bytesWrote);
	if(fres == FR_OK) {
	    yazdi = 1;
	} else {
	    yazdi = 0;
	}

	f_close(&fil);
}
