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

enum CONTROL_MODE{
	openingControl,
	pressureControl,
};


#define SEGMENTNUM 9

class SOFT_ARM{
public:
	SOFT_ARM();

	SOFT_ARM_SEGMENT armSegments[SEGMENTNUM];

	uint16_t commandsBuffer[SEGMENTNUM][BELLOWNUM];
	struct SENSORDATA sensorData[SEGMENTNUM][BELLOWNUM];
	uint8_t canBusCommand[4];
	void setup();
	void loop();
	void controlPressureAll();
	void writeOpeningAll();
};



#ifdef __cplusplus
}
#endif
#endif /* USERINC_SOFTARM_H_ */
