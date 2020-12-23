/*
 * SoftArm.h
 *
 *  Created on: May 12, 2018
 *      Author: 402072495
 */

#ifndef USERINC_SOFTARM_H_
#define USERINC_SOFTARM_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "PneuDrive.h"
#include "SoftArmSegment.h"
#include <CANbus.h>
#include "messages.h"

enum COMMAND_MODE{
	openingCommandType,
	pressureCommandType,

};


#define SEGMENTNUM 9
#define P_ATM 101000


typedef struct SPIDATA_T_TAG{
	SENSORDATA data[SEGMENTNUM][BELLOWNUM];
	char infos[10];
}SPIDATA_T;

typedef struct SPIDATA_R_TAG{
	COMMANDDATA data[SEGMENTNUM][BELLOWNUM];
	char infos[10];
}SPIDATA_R;


class SOFT_ARM{
public:
	SOFT_ARM();

	SOFT_ARM_SEGMENT armSegments[SEGMENTNUM];

	//local sensor and command buffer
	SENSORDATACOMPACT sensorDataBuffer[SEGMENTNUM][BELLOWNUM];
	SPIDATA_R commandDataBuffer;

	//SPI transfer and receive buffer
	SPIDATA_T sensorData;
	SPIDATA_R commandData;

	uint8_t actuatorOnline[SEGMENTNUM][BELLOWNUM];
	int16_t pressureOffset[SEGMENTNUM][BELLOWNUM];
	int16_t laserOffset[SEGMENTNUM][BELLOWNUM];

	PRESSURE_SOURCE pSource;
	PRESSURE_SOURCE pSink;

	uint8_t canBusCommand[8];
	void setupChamberPorts();
	void setupPumps(int d1,int a1, int d2, int a2);
	void maintainPressure(float posP, float negP);
	void readPressureAll();
	void zeroPressureAll();
	void zeroPositionAll();
	void writeCommandAll();
	void execInfoCommand(char *);
};




#ifdef __cplusplus
}
#endif
#endif /* USERINC_SOFTARM_H_ */
