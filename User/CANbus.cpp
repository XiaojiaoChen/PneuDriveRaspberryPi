/*
 * myCanbus.c
 *
 *  Created on: 2020年1月7日
 *      Author: Xiaojiao Chen
 *       Email: yii402072495@gmail.com
 */

#include <CANbus.h>
#include "softArm.h"
struct CANBUS_HANDLE canbus;
extern SOFT_ARM softArm;


/***************************     Can bus        ***************************/
/***************************                     ***************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
void canConfig(){
	canbus.CanHandle=hcan1;
	CAN_FilterTypeDef  sFilterConfig;
	/*##-2- Configure the CAN Filter ###########################################*/
	/*For single CAN instance(14 dedicated filter banks)*/
	for(int i=0;i<14;i++){
		sFilterConfig.FilterBank = i;
		sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
		sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
		sFilterConfig.FilterIdHigh = 4*i;
		sFilterConfig.FilterIdLow = 4*i+1;
		sFilterConfig.FilterMaskIdHigh = 4*i+2;
		sFilterConfig.FilterMaskIdLow = 4*i+3;
		sFilterConfig.FilterFIFOAssignment = i/2;//CAN_RX_FIFO0=0; CAN_RX_FIFO1=1
		sFilterConfig.FilterActivation = ENABLE;
		sFilterConfig.SlaveStartFilterBank = 14;

		if (HAL_CAN_ConfigFilter(&canbus.CanHandle, &sFilterConfig) != HAL_OK)
		{
		/* Filter configuration Error */
		Error_Handler();
		}
	}


	/*##-3- Start the CAN peripheral ###########################################*/
	if (HAL_CAN_Start(&canbus.CanHandle) != HAL_OK)
	{
	/* Start Error */
	Error_Handler();
	}

	/*##-4- Activate CAN RX notification #######################################*/
	if (HAL_CAN_ActivateNotification(&canbus.CanHandle, CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
	{
	/* Notification Error */
	Error_Handler();
	}


}
/*No need to use this right now, left for sending commands to nodes in the future*/
void canSend()
{
	/*##-Configure Transmission Packet #####################################*/
	canbus.TxHeader.StdId = 0x000;
	canbus.TxHeader.ExtId = 0x01;
	canbus.TxHeader.RTR = CAN_RTR_DATA;
	canbus.TxHeader.IDE = CAN_ID_STD;
	canbus.TxHeader.TransmitGlobalTime = DISABLE;

	canbus.TxHeader.DLC = 4;
	canbus.TxData=(uint8_t *)(softArm.canBusCommand);


	/* Start the Transmission process */
	if (HAL_CAN_AddTxMessage(&canbus.CanHandle, &canbus.TxHeader, canbus.TxData, &canbus.TxMailbox) != HAL_OK)
	{
	  /* Transmission request Error */
	  Error_Handler();
	}
}



/*Automaticall called when can bus RxFiFo get message. Move the received message to corresponding buffer accoding to the RxHeader ID*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  /* Get RX message */
  canbus.RxData=(uint8_t *)(&softArm.sensorData[canbus.RxHeader.StdId]);
  if (HAL_CAN_GetRxMessage(&canbus.CanHandle, CAN_RX_FIFO0, &canbus.RxHeader, canbus.RxData) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }

}

/*Automaticall called when can bus RxFiFo get message. Move the received message to corresponding buffer accoding to the RxHeader ID*/
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  /* Get RX message */
	canbus.RxData=(uint8_t *)(&softArm.sensorData[canbus.RxHeader.StdId]);
  if (HAL_CAN_GetRxMessage(&canbus.CanHandle, CAN_RX_FIFO1, &canbus.RxHeader, canbus.RxData) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }


}



