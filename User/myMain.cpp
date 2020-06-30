/*
 * Interface_example_softArm.cpp
 *
 *  Created on: Jan 16, 2019
 *      Author: Xiaojiao Chen
 *		 Email: chen2014@connect.hku.hk
 *	Laboratory: Bio-Control Lab
 * Orgnization: the University of Hong Kong
 */

#include <PneuDrive.h> // always include this header in your own code.
#include <ROSSerialNode.h>
#include "spiSlave.h"
#include <SoftArm.h>

SOFT_ARM softArm;


void setup() {
	/*setup 1000Hz control loop*/
	setPeriodControlLoop(10);

	/*setup 50Hz serial display loop*/
	setPeriodSendLoop(100);

	/*soft arm chambers' PWM port mapping*/
	softArm.setupChamberPorts();

	/*soft arm Pumps' port mapping*/
	softArm.setupPumps(0,BUILTIN_ANA_IN_NUM+0,1,BUILTIN_ANA_IN_NUM+1);

	/*start canBus receive*/
	canConfig();

	/**********start the SPI slave in DMA*****/
	spiSlaveStart();

	/*********start the rosserial node*******/
	rosNodeSetup();
}

void loop() {

	/***************Handle string commands from the raspberry pi********************/
	//softArm.execInfoCommand(softArm.commandData.infos);
	softArm.maintainPressure(100000,-60000);

	//update chamber's pressure from the CANbus************************/
	softArm.readPressureAll();

	/**Write the command of each chamber, either pressure or opening type*/
	softArm.writeCommandAll();

	/*rosserial publish**/
	rosNodePub();

	/**rosserial spinonce routine***/
	rosNodeSpinOnce();
}

/*serial output using DMA*/
void serialDisplay() {
	//printf("Time: %d\r\n",millis());
	printf("Time:%10lu    PressureCommand:Pressure(HPa)    Distance(mm)  pSource:%d HPa  pSink:%d HPa\r\n",
			millis(),(int)(softArm.pSource.pressure/100),(int)(softArm.pSink.pressure/100));
	for (int i = 0; i < SEGMENTNUM; i++) {
		printf(
				"     %4d:%4d   %4d:%4d   %4d:%4d   %4d:%4d   %4d:%4d   %4d:%4d    |     %2u,  %2u, %2u, %2u, %2u, %2u\r\n",
				softArm.commandData.data[i][0].values[0],(int)(softArm.armSegments[i].bellows[0]->pressure/100),
				softArm.commandData.data[i][1].values[0],(int)(softArm.armSegments[i].bellows[1]->pressure/100),
				softArm.commandData.data[i][2].values[0],(int)(softArm.armSegments[i].bellows[2]->pressure/100),
				softArm.commandData.data[i][3].values[0],(int)(softArm.armSegments[i].bellows[3]->pressure/100),
				softArm.commandData.data[i][4].values[0],(int)(softArm.armSegments[i].bellows[4]->pressure/100),
				softArm.commandData.data[i][5].values[0],(int)(softArm.armSegments[i].bellows[5]->pressure/100),
				softArm.sensorData.data[i][0].distance,
				softArm.sensorData.data[i][1].distance,
				softArm.sensorData.data[i][2].distance,
				softArm.sensorData.data[i][3].distance,
				softArm.sensorData.data[i][4].distance,
				softArm.sensorData.data[i][5].distance);
	}
}

//called when serial input receive string with ending '\r\n'
void serial3Callback(char *pSerialReceiveBuffer) {
	softArm.execInfoCommand(pSerialReceiveBuffer);
//	if (pSerialReceiveBuffer[0] == 'k') {
//		//	/**Write command for each actuator, q&r of kalman filter for laser, */
////		softArm.canBusCommand[0] = 0xFF;
////		softArm.canBusCommand[1] = 0xEF;
////		softArm.canBusCommand[2] = 0xFE;
////		softArm.canBusCommand[3] = 0xAF;
////		canSend();
//	}
}

