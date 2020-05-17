/*
 * SoftArmSegment.h
 *
 *  Created on: May 12, 2018
 *      Author: 402072495
 */

#ifndef USERINC_SOFTARMSEGMENT_H_
#define USERINC_SOFTARMSEGMENT_H_

#ifdef __cplusplus
extern "C" {
#endif



#include "PneuDrive.h"

#define BELLOWNUM 6

typedef struct IMUFRAME_STRUCT{
	uint8_t headerEuler[2];
	int16_t Rx;
	int16_t Ry;
	int16_t Rz;
	int16_t temperature;
	uint8_t chekcum1;
	uint8_t headerQuotanion[2];
	int16_t q0;
	int16_t q1;
	int16_t q2;
	int16_t q3;
	uint8_t chekcum2;
}__attribute__((packed)) IMUFRAME;

typedef struct IMUDATA_STRUCT{
	float angleX;
	float angleY;
	float angleZ;
	float q0;
	float q1;
	float q2;
	float q3;
}__attribute__((packed)) IMUDATA;


class SOFT_ARM_SEGMENT{
public:
	SOFT_ARM_SEGMENT(int num=BELLOWNUM);

	void attach(int num,int PWMPort1,int PWMPort2,int AnalogPort);

	float readPressure(int num);
	void setPressureCommand(int num,float pre);

	void readPressureAll();
	void controlPressureAll();
	void writeOpeningAll(float op);

	float pressure[BELLOWNUM];
	float pressureFil[BELLOWNUM];

	float pressureD[BELLOWNUM];


	float frequency;
	float pressureBase;
	float openingBase;

	PNEUDRIVE *basePlatform;
	CHAMBER *bellows[BELLOWNUM];

	float pressureCommandMin[BELLOWNUM];
	float pressureCommandMax[BELLOWNUM];

	int frequencyDirty;


	float readLength();
	void readIMU(char *pBuffer);
	void quatern2AB();

	IMUDATA imuData;
	int lengthAnalogPort;
	float alpha;
	float beta;
	float length;
	float lengthRaw;
};

#ifdef __cplusplus
}
#endif
#endif /* USERINC_SOFTARMSEGMENT_H_ */
