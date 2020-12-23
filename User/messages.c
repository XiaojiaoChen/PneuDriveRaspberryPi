/*
 * messages.c
 *
 *  Created on: Mar 6, 2020
 *      Author: Xiaojiao Chen
 *       Email: yii402072495@gmail.com
 */
#include "messages.h"
#include "stdlib.h"

#if COMPACT_VERSION_PRESSURE_HPA==1
	#define COMPACT_SHIFT_BIT 2
#else
	#define COMPACT_SHIFT_BIT 1
#endif

static unsigned int fastSqrt32(unsigned long n);

void packQuaternion(QUATERNION *qOri,QUATERNIONCOMPACT *qCom){
	uint16_t maxLoc=0;
	for(int i=0;i<4;i++){
		if(i>0 && (abs(qOri->imuData[i])>abs(qOri->imuData[maxLoc])))
			maxLoc=i;
	}
	if(maxLoc==0)
	{
		qCom->imuData0=((uint16_t)qOri->imuData[1])>>COMPACT_SHIFT_BIT;
		qCom->imuData1=((uint16_t)qOri->imuData[2])>>COMPACT_SHIFT_BIT;
		qCom->imuData2=((uint16_t)qOri->imuData[3])>>COMPACT_SHIFT_BIT;
	}
	else if (maxLoc==1){
		qCom->imuData0=((uint16_t)qOri->imuData[0])>>COMPACT_SHIFT_BIT;
		qCom->imuData1=((uint16_t)qOri->imuData[2])>>COMPACT_SHIFT_BIT;
		qCom->imuData2=((uint16_t)qOri->imuData[3])>>COMPACT_SHIFT_BIT;
	}
	else if(maxLoc==2){
		qCom->imuData0=((uint16_t)qOri->imuData[0])>>COMPACT_SHIFT_BIT;
		qCom->imuData1=((uint16_t)qOri->imuData[1])>>COMPACT_SHIFT_BIT;
		qCom->imuData2=((uint16_t)qOri->imuData[3])>>COMPACT_SHIFT_BIT;
	}
	else{
		qCom->imuData0=((uint16_t)qOri->imuData[0])>>COMPACT_SHIFT_BIT;
		qCom->imuData1=((uint16_t)qOri->imuData[1])>>COMPACT_SHIFT_BIT;
		qCom->imuData2=((uint16_t)qOri->imuData[2])>>COMPACT_SHIFT_BIT;
	}
	qCom->maxLocHigh=maxLoc>>1;
	qCom->maxLocLow=maxLoc;
	qCom->maxSign=((uint16_t)(((uint16_t)qOri->imuData[maxLoc])&0x8000))>>15;
}

void unpackQuaternion(QUATERNIONCOMPACT *qCom,QUATERNION *qOri){
	int32_t lastNumOri=0;
	uint16_t maxLoc=(uint16_t)(qCom->maxLocHigh<<1 | qCom->maxLocLow);
	int16_t qRes0=(int16_t)(((uint16_t)qCom->imuData0)<<COMPACT_SHIFT_BIT);
	int16_t qRes1=(int16_t)(((uint16_t)qCom->imuData1)<<COMPACT_SHIFT_BIT);
	int16_t qRes2=(int16_t)(((uint16_t)qCom->imuData2)<<COMPACT_SHIFT_BIT);

	uint32_t sum=qRes0*qRes0+qRes1*qRes1+qRes2*qRes2;
	const int maxNum=(uint32_t)(1<<30);
	lastNumOri=(sum>=maxNum?0:fastSqrt32((uint32_t)(maxNum-sum)));
	int16_t lastNum=qCom->maxSign?-lastNumOri:lastNumOri;
	if(maxLoc==0)
	{
		qOri->imuData[1]=qRes0;
		qOri->imuData[2]=qRes1;
		qOri->imuData[3]=qRes2;
		qOri->imuData[0]=lastNum;
	}
	else if (maxLoc==1){
		qOri->imuData[0]=qRes0;
		qOri->imuData[2]=qRes1;
		qOri->imuData[3]=qRes2;
		qOri->imuData[1]=lastNum;
	}
	else if(maxLoc==2){
		qOri->imuData[0]=qRes0;
		qOri->imuData[1]=qRes1;
		qOri->imuData[3]=qRes2;
		qOri->imuData[2]=lastNum;
	}
	else{
		qOri->imuData[0]=qRes0;
		qOri->imuData[1]=qRes1;
		qOri->imuData[2]=qRes2;
		qOri->imuData[3]=lastNum;
	}
}


void decodeSensorData(SENSORDATACOMPACT *scom, SENSORDATA *s,int16_t pOff,int16_t disOff) {

	s->pressure=(int16_t)(unpackPressure(scom))-pOff; //gauge hpa
	s->distance= (int16_t)(unpackPosition(scom))-disOff;
	unpackQuaternion(&(scom->quaternionCom),&(s->quaternion));
}
uint16_t unpackPressure(SENSORDATACOMPACT *scom){
#if COMPACT_VERSION_PRESSURE_HPA==1
	return scom->pressure;
#else
	return scom->pressure*10;
#endif
}
uint16_t unpackPosition(SENSORDATACOMPACT *scom){
	uint16_t pos=0;
#if COMPACT_VERSION_PRESSURE_HPA==1
	pos= ((uint16_t)(scom->distance<<3)
				+(uint16_t)(scom->quaternionCom.distanceBit2<<2)
				+(uint16_t)(scom->quaternionCom.distanceBit1<<1)
				+(uint16_t)(scom->quaternionCom.distanceBit0));
#else
	pos=scom->distance;
#endif
	return pos;
}

static unsigned int fastSqrt32(unsigned long n)
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


