/*
 * messages.c
 *
 *  Created on: Mar 6, 2020
 *      Author: Xiaojiao Chen
 *       Email: yii402072495@gmail.com
 */
#include "messages.h"
#include "stdlib.h"
static unsigned int fastSqrt32(unsigned long n);


void packQuaternion(int16_t *qOri,struct QUATERNIONCOMPACT *qCom){
	uint16_t maxLoc=0;
	for(int i=0;i<4;i++){
		if(i>0 && (abs(qOri[i])>abs(qOri[maxLoc])))
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
