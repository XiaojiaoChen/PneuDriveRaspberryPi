/*
 * SoftArm.c
 *
 *  Created on: May 12, 2018
 *      Author: 402072495
 */
#include <SoftArmSegment.h>
#include "stdio.h"

const static float Patm=101325;
const static float PaPerPSI=6895;


/*************************SOFT ARM**************************
 *
 ***********************************************************/
SOFT_ARM_SEGMENT::SOFT_ARM_SEGMENT(int num)
{


	length = 0.367;
	alpha = 0;
	beta = 0;

	basePlatform = new PNEUDRIVE(num);
	for(int i=0;i<BELLOWNUM;i++)
	{
		bellows[i]=basePlatform->chambers[i];
	}

}



float SOFT_ARM_SEGMENT::readPressure(int num)
{
	pressure[num]=basePlatform->chambers[num]->readPressure();
	pressureFil[num]=basePlatform->chambers[num]->pressureFil;
	return pressure[num];
}
void SOFT_ARM_SEGMENT::readPressureAll()
{
	for (int i = 0; i < BELLOWNUM; i++) {
		readPressure(i);
	}
}

void SOFT_ARM_SEGMENT::writeOpeningAll(float op)
{
	for (int i = 0; i < BELLOWNUM; i++) {
		basePlatform->chambers[i]->writeOpening(op);
	}
}

void SOFT_ARM_SEGMENT::controlPressureAll()
{
	for (int i = 0; i < BELLOWNUM; i++) {
		pressureD[i]=CONSTRAIN(pressureD[i],pressureCommandMin[i],pressureCommandMax[i]);
		basePlatform->chambers[i]->writePressure(pressureD[i]);
	}
}
void SOFT_ARM_SEGMENT::setPressureCommand(int num,float pre)
{
	pressureD[num]=pre;
	pressureD[num]=CONSTRAIN(pressureD[num],pressureCommandMin[num],pressureCommandMax[num]);
}




float SOFT_ARM_SEGMENT::readLength() {
	float vol=AnalogRead(lengthAnalogPort);
	lengthRaw= MAP(vol,0,10.0,0,1);
	length=(lengthRaw-0.02)*(alpha/2/sin(alpha/2))+alpha*0.02*cos(beta);
	return length;
}

void SOFT_ARM_SEGMENT::readIMU(char *pBuffer) {
	IMUFRAME *pFrame=(IMUFRAME *)pBuffer;
	if(pFrame->headerEuler[0]==0x55 && pFrame->headerEuler[1]==0x53)
	{
		imuData.angleX=(pFrame->Rx)/32768.0f*180;
		imuData.angleY=(pFrame->Ry)/32768.0f*180;
		imuData.angleZ=(pFrame->Rz)/32768.0f*180;
		imuData.q0=(pFrame->q0)/32768.0f;
		imuData.q1=(pFrame->q1)/32768.0f;
		imuData.q2=(pFrame->q2)/32768.0f;
		imuData.q3=(pFrame->q3)/32768.0f;
		imuData.q0_int=pFrame->q0;
		imuData.q1_int=pFrame->q1;
		imuData.q2_int=pFrame->q2;
		imuData.q3_int=pFrame->q3;
	}
	quatern2AB();


}

void SOFT_ARM_SEGMENT::quatern2AB(){
	float n1=2*imuData.q0*imuData.q2 + 2*imuData.q1*imuData.q3;
	float n2=2*imuData.q2*imuData.q3 - 2*imuData.q0*imuData.q1;
	float n3=imuData.q0*imuData.q0 - imuData.q1*imuData.q1 - imuData.q2*imuData.q2 + imuData.q3*imuData.q3;

	  n1=CONSTRAIN(n1,-1,1);
	  n2=CONSTRAIN(n2,-1,1);
	  n3=CONSTRAIN(n3,-1,1);

	  alpha=acos(-n3);
	  alpha=CONSTRAIN(alpha,0.001,M_PI);//avoid singularity
	  beta=atan2(-n2,n1);
	  if(alpha<0.13)//the smaller alpha is, the larger error beta would have.
		  beta=0;
}
