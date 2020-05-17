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

	/*start canBus receive*/
	canConfig();

	/**********start the SPI slave in DMA*****/
	spiSlaveStart();

}

void loop() {

	/***************Handle string commands from the raspberry pi********************/
	softArm.execInfoCommand(softArm.commandData.infos);

	//update chamber's pressure from the CANbus************************/
	softArm.readSensorAll();



	/**Write the command of each chamber, either pressure or opening type*/
	softArm.writeCommandAll();

}

/*serial output using DMA*/
void serialDisplay() {
	//printf("Time: %d\r\n",millis());
	printf("Time:%10lu    PressureCommand:Pressure(KPa)    Distance(mm)\r\n",
			millis());
	for (int i = 0; i < SEGMENTNUM; i++) {
		printf(
				"     %hd:%hd   %hd:%hd   %hd:%hd   %hd:%hd   %hd:%hd   %hd:%hd    |     %hu,  %hu, %hu, %hu, %hu, %hu\r\n",
				softArm.commandData.data[i][0].values[0],(int)(softArm.armSegments[i].bellows[0]->pressure),
				softArm.commandData.data[i][1].values[0],(int)(softArm.armSegments[i].bellows[1]->pressure),
				softArm.commandData.data[i][2].values[0],(int)(softArm.armSegments[i].bellows[2]->pressure),
				softArm.commandData.data[i][3].values[0],(int)(softArm.armSegments[i].bellows[3]->pressure),
				softArm.commandData.data[i][4].values[0],(int)(softArm.armSegments[i].bellows[4]->pressure),
				softArm.commandData.data[i][5].values[0],(int)(softArm.armSegments[i].bellows[5]->pressure),
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

	if (pSerialReceiveBuffer[0] == 'k') {
		//	/**Write command for each actuator, q&r of kalman filter for laser, */
//		softArm.canBusCommand[0] = 0xFF;
//		softArm.canBusCommand[1] = 0xEF;
//		softArm.canBusCommand[2] = 0xFE;
//		softArm.canBusCommand[3] = 0xAF;
//		canSend();
	}
}

void serial2Callback(char *pSerialReceiveBuffer) {
	for(int j=0;j<SEGMENTNUM;j++){
		softArm.armSegments[j].readIMU(pSerialReceiveBuffer);
	}
}
