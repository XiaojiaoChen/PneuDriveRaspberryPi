/*
 * spiSlave.c
 *
 *  Created on: Mar 9, 2020
 *      Author: Xiaojiao Chen
 *       Email: yii402072495@gmail.com
 */

#include "spiSlave.h"
#include "softArm.h"
#include "spi.h"
#include "string.h"
#include <inttypes.h>

extern SOFT_ARM softArm;

SPI_HandleTypeDef *hspiSlave=&hspi1;


void spiSlaveStart(){
	HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t *)(&softArm.sensorData),  (uint8_t *)(&softArm.commandDataBuffer), sizeof(SPIDATA_R));
}



void slaveSPITxRxCpltCallback(SPI_HandleTypeDef *hspi){
	if(hspi==hspiSlave){

		/*Success on receiving data. Update the actual commandData*/
		memcpy(&(softArm.commandData),&(softArm.commandDataBuffer),sizeof(SPIDATA_R));

		/*Re-arm the SPI slave, ready to receive and transmit*/
		HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t *)(&softArm.sensorData),  (uint8_t *)(&softArm.commandDataBuffer), sizeof(SPIDATA_R));
	}
}

void slaveSPIErrorCallback(SPI_HandleTypeDef *hspi){
	if(hspi==hspiSlave){
		HAL_SPI_StateTypeDef hspistate=HAL_SPI_GetState(hspi);
		uint32_t hspierror=HAL_SPI_GetError(hspi);
		printf("Error communication with RPI4, SPI state: %d, SPI Errorcode:%d\r\n",hspistate,hspierror);
		/*If an error occurs, we only need to re-arm the SPI slave, ready to receive and transmit*/
		HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t *)(&softArm.sensorData),  (uint8_t *)(&softArm.commandDataBuffer), sizeof(SPIDATA_R));
	}
}

