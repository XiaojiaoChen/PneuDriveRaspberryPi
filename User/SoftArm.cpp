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


void SOFT_ARM::setup()
{
	/*Every actuator is refered to with two numbers, segNum(0-8) and bellowNum(0-5)*/

	for(int j=0;j<SEGMENTNUM;j++){
		SOFT_ARM_SEGMENT* armSegCur=&armSegments[j];
		for(int i=0;i<BELLOWNUM;i++)
		{
			CHAMBER *bellowCur=armSegCur->bellows[i];
			int pwmPortOffset=BUILTIN_PWM_NUM+j*16;
			/*analog port is treated as the overall No. in our arm.*/
			bellowCur->attach(pwmPortOffset+i*2, pwmPortOffset+i*2+1, j*6+i);
			bellowCur->writeOpening(0);
		}
	}

	/**********The sensor feedback are automatically received from nodes, and stored in sensorData[segNum][bellowNum] for every actuator.*/
	canConfig();

	/**********TO DO. The stm32 SPI slave should be implemented with DMA, which automatically transfer the sensorData and upadte the command*****/
	SPISlaveConfig();

}

void SOFT_ARM::loop()
{

	/*With automatically updated sensor info and command info, we just need decide how to decode the command source and take actions accordingly*/
	for(int j=0;j<SEGMENTNUM;j++){
		SOFT_ARM_SEGMENT* armSegCur=&armSegments[j];
		for(int i=0;i<BELLOWNUM;i++)
		{
			CHAMBER *bellowCur=armSegCur->bellows[i];

			armSegCur->bellows[i]->pressure=sensorData[j][i].pressure*100-101325;

			int commandMode=commandsBuffer[j][i]&0xC000;
			if (commandMode==openingControl){
				float openingCommandTemp=((int16_t)(commandsBuffer[j][i]<<2))*0.00012207f;
				bellowCur->writeOpening(openingCommandTemp);
			}
			else if (commandMode==pressureControl){
				float pressureCommandTemp=((uint16_t)(commandsBuffer[j][i]<<2))*100;
				bellowCur->writePressure(pressureCommandTemp);
			}
		}
	}
}

void SOFT_ARM::controlPressureAll()
{

}
void SOFT_ARM::writeOpeningAll(){

}

