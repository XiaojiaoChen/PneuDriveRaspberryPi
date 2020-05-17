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
		printf("Error SPI 1 communication with RPI4\r\n");
		/*If an error occurs, we only need to re-arm the SPI slave, ready to receive and transmit*/
		HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t *)(&softArm.sensorData),  (uint8_t *)(&softArm.commandDataBuffer), sizeof(SPIDATA_R));
	}
}

