/*
 * messages.h
 *
 *  Created on: Mar 6, 2020
 *      Author: Xiaojiao Chen
 *       Email: yii402072495@gmail.com
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f7xx.h"

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


 /*SENSORDATA must have the same length with COMMANDDATA for SPI fullduplex communication*/
 struct SENSORDATA{
 uint16_t pressure:12, distanceBit6_3:4;
 struct QUATERNIONCOMPACT quaternionCom;
 };


 struct COMMANDDATA{
 uint16_t values[3];
 uint16_t commandType;
 };







/*packQ should be used in sender, i.e. actuator nodes*/
void packQuaternion(int16_t* qOri,struct QUATERNIONCOMPACT *qCom);

/*unpackQ should be used in decoder, i.e. PC host*/
void unpackQuaternion(struct QUATERNIONCOMPACT *qCom,int16_t* qOri);









#ifdef __cplusplus
}
#endif
#endif /* MESSAGES_H_ */
