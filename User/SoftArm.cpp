/*
 * SoftArm.c
 *
 *  Created on: May 12, 2018
 *      Author: 402072495
 */
#include "SoftArm.h"
#include "stdio.h"




/*************************SOFT ARM**************************
 *
 ***********************************************************/
SOFT_ARM::SOFT_ARM()
{

}


void SOFT_ARM::setupChamberPorts()
{
	/*Every actuator is refered to with two numbers, segNum(0-8) and bellowNum(0-5)*/
	float initOpening=0;
	for(int j=0;j<SEGMENTNUM;j++){
		SOFT_ARM_SEGMENT* armSegCur=&armSegments[j];
		for(int i=0;i<BELLOWNUM;i++)
		{
			CHAMBER *bellowCur=armSegCur->bellows[i];
			int pwmPortOffset=BUILTIN_PWM_NUM+j*16; //each pwm board has 16*3 ports, while we only need 6*2*3 on each
			/*analog port is treated as the overall No. in our arm.*/

			if(j==1 || j==2 || j==6){
				bellowCur->attach(pwmPortOffset+BELLOWNUM+i, pwmPortOffset+i, j*BELLOWNUM+i);
			}
			else{
				bellowCur->attach(pwmPortOffset+i, pwmPortOffset+BELLOWNUM+i, j*BELLOWNUM+i);
			}

			bellowCur->writeOpening(initOpening);


			HAL_Delay(10);
		}
	}
	int j=2;
	for(int i=0;i<BELLOWNUM;i++)
			{
				CHAMBER *bellowCur=armSegments[j].bellows[i];
				int pwmPortOffset=BUILTIN_PWM_NUM+j*16; //each pwm board has 16*3 ports, while we only need 6*2*3 on each
				bellowCur->attach(4*16+pwmPortOffset+BELLOWNUM+i, 4*16+pwmPortOffset+i, j*BELLOWNUM+i);//segment 2 changed to segment 6.
				bellowCur->writeOpening(initOpening);
			}
	j=5;
	for(int i=0;i<BELLOWNUM;i++)
				{
					CHAMBER *bellowCur=armSegments[j].bellows[i];
					int pwmPortOffset=BUILTIN_PWM_NUM+j*16; //each pwm board has 16*3 ports, while we only need 6*2*3 on each
					bellowCur->attach(2*16+pwmPortOffset+i, 2*16+pwmPortOffset+BELLOWNUM+i, j*BELLOWNUM+i);//segment 5 changed to segment 7.
					bellowCur->writeOpening(initOpening);
				}
	j=6;
	for(int i=0;i<BELLOWNUM;i++)
			{
				CHAMBER *bellowCur=armSegments[j].bellows[i];
				int pwmPortOffset=BUILTIN_PWM_NUM+j*16; //each pwm board has 16*3 ports, while we only need 6*2*3 on each
				bellowCur->attach(pwmPortOffset+BELLOWNUM+i-4*16, pwmPortOffset+i-4*16, j*BELLOWNUM+i);//segment 2 changed to segment 6.
				bellowCur->writeOpening(initOpening);
			}
	j=7;
	for(int i=0;i<BELLOWNUM;i++)
				{
					CHAMBER *bellowCur=armSegments[j].bellows[i];
					int pwmPortOffset=BUILTIN_PWM_NUM+j*16; //each pwm board has 16*3 ports, while we only need 6*2*3 on each
					bellowCur->attach(pwmPortOffset+i-2*16, pwmPortOffset+BELLOWNUM+i-2*16, j*BELLOWNUM+i);//segment 7 changed to segment 5.
					bellowCur->writeOpening(initOpening);
				}
//	j=0;
//
//		CHAMBER *bellowCur=armSegments[j].bellows[0];
//		int pwmPortOffset=BUILTIN_PWM_NUM+j*16; //each pwm board has 16*3 ports, while we only need 6*2*3 on each
//		bellowCur->attach(5*16+pwmPortOffset, 5*16+pwmPortOffset+BELLOWNUM, j*BELLOWNUM);//segment 2 changed to segment 6.


}

void SOFT_ARM::zeroPressureAll() {
	for(int j=0;j<SEGMENTNUM;j++){
		for(int i=0;i<BELLOWNUM;i++){
			armSegments[j].bellows[i]->zeroPressure();
		}
	}
}

void SOFT_ARM::zeroPositionAll() {
	for(int j=0;j<SEGMENTNUM;j++){
		for(int i=0;i<BELLOWNUM;i++){
			armSegments[j].bellows[i]->zeroPosition();
		}
	}
}



void SOFT_ARM::readPressureAll()
{
	//Read pressure information from sensorData(from CANbus) as the Chamber's pressure
	for(int j=0;j<SEGMENTNUM;j++){
		for(int i=0;i<BELLOWNUM;i++){
			armSegments[j].bellows[i]->readPressureCan(unpackPressure(&(sensorDataBuffer[j][i])));//original absolute hpa
			armSegments[j].bellows[i]->readPositionCan(unpackPosition(&(sensorDataBuffer[j][i])));//original absolute hpa

		}
	}
}

void SOFT_ARM::writeCommandAll()
{
	for(int j=0;j<SEGMENTNUM;j++){
		SOFT_ARM_SEGMENT* armSegCur=&armSegments[j];
			for(int i=0;i<BELLOWNUM;i++){
				CHAMBER *bellowCur=armSegCur->bellows[i];
				//write pressure command from commandData to the chamber
				if(commandData.data[j][i].commandType==pressureCommandType){
					float pressureCommandTemp=commandData.data[j][i].values[0]*100;//gauge PA
					bellowCur->writePressure(pressureCommandTemp);
				}
				else if(commandData.data[j][i].commandType==openingCommandType){
					float openingCommandTemp=((int16_t)commandData.data[j][i].values[0])*3.0517578125e-5;//values[0]/32767
					bellowCur->writeOpening(openingCommandTemp);
				}
			}
//		else{
//			for(int i=0;i<BELLOWNUM;i++){
//				CHAMBER *bellowCur=armSegCur->bellows[i];
//				if(i%2)
//					bellowCur->writeOpening(0.8);
//				else
//					bellowCur->writeOpening(-0.8);
//				}
//		}
	}
}

void SOFT_ARM::maintainPressure(float posP, float negP) {
	pSource.maintainPressure(posP,posP+10000);
	pSink.maintainPressure(negP-10000,negP);
}

void SOFT_ARM::execInfoCommand(char *infoBuf){
	if(infoBuf[0]=='z'){
		zeroPressureAll();
		zeroPositionAll();
	}if(infoBuf[0]=='o'){

	}
}

void SOFT_ARM::execInfoRaspberryPi(){
	execInfoCommand(commandData.infos);
}

void SOFT_ARM::setupPumps(int d1,int a1, int d2, int a2) {
	pSource.attach(d1, a1);
	pSink.attach(d2, a2);

}
