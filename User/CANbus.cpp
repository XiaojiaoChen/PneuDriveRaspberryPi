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



void packQuaternion(int16_t* qOri,struct QUATERNIONCOMPACT *qCom){
	uint16_t maxLoc=0;
	for(int i=0;i<4;i++){
		if(i>0 && abs(qOri[i])>abs(qOri[maxLoc]))
			maxLoc=i;
	}
	if(maxLoc==0)
	{
		qCom->imuData0=((uint16_t)qOri[1])>>2;
		qCom->imuData1=((uint16_t)qOri[2])>>2;
		qCom->imuData2=((uint16_t)qOri[3])>>2;
	}
	else if (maxLoc==1){
		qCom->imuData0=((uint16_t)qOri[0])>>2;
		qCom->imuData1=((uint16_t)qOri[2])>>2;
		qCom->imuData2=((uint16_t)qOri[3])>>2;
	}
	else if(maxLoc==2){
		qCom->imuData0=((uint16_t)qOri[0])>>2;
		qCom->imuData1=((uint16_t)qOri[1])>>2;
		qCom->imuData2=((uint16_t)qOri[3])>>2;
	}
	else{
		qCom->imuData0=((uint16_t)qOri[0])>>2;
		qCom->imuData1=((uint16_t)qOri[1])>>2;
		qCom->imuData2=((uint16_t)qOri[2])>>2;
	}
	qCom->maxLocHigh=maxLoc>>1;
	qCom->maxLocLow=maxLoc;
	qCom->maxSign=((uint16_t)(((uint16_t)qOri[maxLoc])&0x8000))>>15;
}
unsigned int fastSqrt32(unsigned long n)
{
    unsigned int c = 0x8000;
    unsigned int g = 0x8000;

    for(;;) {
        if(g*g > n)
            g ^= c;
        c >>= 1;
        if(c == 0)
            return g;
        g |= c;
    }
}
void unpackQuaternion(struct QUATERNIONCOMPACT *qCom,int16_t* qOri){
	int32_t lastNumOri=0;
	uint16_t maxLoc=(uint16_t)(qCom->maxLocHigh<<1 | qCom->maxLocLow);
	int16_t qRes0=(int16_t)(((uint16_t)qCom->imuData0)<<2);
	int16_t qRes1=(int16_t)(((uint16_t)qCom->imuData1)<<2);
	int16_t qRes2=(int16_t)(((uint16_t)qCom->imuData2)<<2);

	uint32_t sum=qRes0*qRes0+qRes1*qRes1+qRes2*qRes2;
	const int maxNum=(uint32_t)(1<<30);
	lastNumOri=(sum>=maxNum?0:fastSqrt32((uint32_t)(maxNum-sum)));
	int16_t lastNum=qCom->maxSign?-lastNumOri:lastNumOri;
	if(maxLoc==0)
	{
		qOri[1]=qRes0;
		qOri[2]=qRes1;
		qOri[3]=qRes2;
		qOri[0]=lastNum;
	}
	else if (maxLoc==1){
		qOri[0]=qRes0;
		qOri[2]=qRes1;
		qOri[3]=qRes2;
		qOri[1]=lastNum;
	}
	else if(maxLoc==2){
		qOri[0]=qRes0;
		qOri[1]=qRes1;
		qOri[3]=qRes2;
		qOri[2]=lastNum;
	}
	else{
		qOri[0]=qRes0;
		qOri[1]=qRes1;
		qOri[2]=qRes2;
		qOri[3]=lastNum;
	}
}

