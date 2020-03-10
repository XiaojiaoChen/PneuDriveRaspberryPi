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
extern SOFT_ARM softArm;


SPI_HandleTypeDef *hspiSlave=&hspi1;


void spiSlaveStart(){
	printf("spi Start\r\n");
	HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t *)(&softArm.sensorData[0][0]),  (uint8_t *)(&softArm.commandData[0][0]), sizeof(softArm.commandData));
}



void slaveSPITxRxCpltCallback(SPI_HandleTypeDef *hspi){
	static uint32_t spiTimes=0;
	if(hspi==hspiSlave){
		/*Re-arm the SPI slave, ready to receive and transmit*/
		printf("spi Cplt %d\r\n",spiTimes);
		HAL_SPI_TransmitReceive_DMA(hspiSlave, (uint8_t *)(&softArm.sensorData[0][0]),  (uint8_t *)(&softArm.commandData[0][0]), sizeof(softArm.commandData));

	}
}

void slaveSPIErrorCallback(SPI_HandleTypeDef *hspi){
	if(hspi==hspiSlave){
		printf("SPI1 error\r\n");
		/*If an error occurs, we also need to re-arm the SPI slave, ready to receive and transmit*/
		HAL_SPI_TransmitReceive_DMA(hspiSlave, (uint8_t *)(&softArm.sensorData[0][0]),  (uint8_t *)(&softArm.commandData[0][0]), sizeof(softArm.commandData));

	}
}

