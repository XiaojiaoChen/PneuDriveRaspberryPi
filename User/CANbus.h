/*
 * myCanbus.h
 *
 *  Created on: 2020年1月7日
 *      Author: Xiaojiao Chen
 *       Email: yii402072495@gmail.com
 */

#ifndef CANBUS_H_
#define CANBUS_H_
#ifdef __cplusplus
 extern "C" {
#endif
#include "PneuDrive.h"
#include "can.h"

 struct QUATERNIONCOMPACT{
	 /*Compressed Quaternion by omitting the largest component.
	  * Other component limited to 15bit:
	  * maxLocHigh and maxLocLow specify which location is the max component(omitted one) (default:q=w+xi+yj+zk)
	  * maxSign is not necessarily needed, for simplicity we added it.
	  */
	 uint16_t imuData0:14,maxLocHigh:1,distanceBit2:1;
	 uint16_t imuData1:14,maxLocLow:1,distanceBit1:1;
	 uint16_t imuData2:14,maxSign:1,distanceBit0:1;
 };

 struct SENSORDATA{
 uint16_t pressure:12, distanceBit6_3:4;
 struct QUATERNIONCOMPACT quaternionCom;
 };

 struct CANBUS_HANDLE{
	CAN_HandleTypeDef     CanHandle;
	CAN_TxHeaderTypeDef   TxHeader;
	CAN_RxHeaderTypeDef   RxHeader;
	uint8_t               *TxData;
	uint8_t               *RxData;
	uint32_t              TxMailbox;
 };

 void canConfig();
 void canSend();
 void canRead();

#ifdef __cplusplus
}
#endif
#endif /* CANBUS_H_ */
