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
PNEUDRIVE pneudrive(2);
int analogPort_source=BUILTIN_ANA_IN_NUM+0;
int analogPort_sink=BUILTIN_ANA_IN_NUM+1;
int analogPort_chamber0=BUILTIN_ANA_IN_NUM+2;
int analogPort_chamber1=BUILTIN_ANA_IN_NUM+3;

int analogPort_laser=BUILTIN_ANA_IN_NUM+7;

float amp=45000;

float offset=-5000;
const uint32_t loopms=10;
float fre=1;
int numUpdateValue=1000;

uint32_t totalNum=0;

float length=0;
float pd;
int32_t loop_gogogo=1;
uint32_t localCounter=0;
uint32_t t0,tcur;
int8_t t0flag=0;
float externPd=0;

float upstreamPressure=110000;
float downstreamPressure=-70000;
float sinbase=0;
float ratio=0.22;
int32_t direction=1;
float chamber0p=0;
float chamber1p=0;

PRESSURE_SOURCE pSink2(7,analogPort_sink);

void setup() {


	/*setup 1000Hz control loop*/
	setPeriodControlLoop(loopms);

	/*setup 50Hz serial display loop*/
	setPeriodSendLoop(50);



	/*Configure the pump port and sensor paramter of the source*/
	pneudrive.pSource.attach(0,analogPort_source);

	/*Configure the pump port and sensor parameter of the sink*/
	pneudrive.pSink.attach(1,analogPort_sink);
	pSink2.attach(7, analogPort_sink);

	/*Attach the valves and pressure sensors of chamber 0 to corresponding ports.
	 *Set the pressure sensor range. The parameter list are (vMin, vMax, pMin, pMax).
	 * vMin vMax have the unit of voltage. pMin pMax use the gauge pressure, has the unit of Pa. For example, atmosphere is 0, and vacuum is -101325.*/
	pneudrive.chambers[0]->attach(0, 2,analogPort_chamber0);
	pneudrive.chambers[1]->attach(1, 3,analogPort_chamber1);

	numUpdateValue=1000/loopms/fre;
}

float readLength(uint8_t port){
	return (AnalogRead(port)*14-35);  //135-v/5.0*(35-(-35));
}

void loop() {

	/*Update sensor data of chamber 0*/
	pneudrive.pSource.readPressure();
	pneudrive.pSink.readPressure();

	chamber0p=pneudrive.readPressure(0);
	pneudrive.chambers[1]->readPressureExt(chamber0p);//using the chamber0 's pressure as the chamber 1's pressure

	length=readLength(analogPort_laser);

	/*customized switch flag*/
	if (loop_gogogo) {

		if(!t0flag){
			t0=millis();
			t0flag=1;
		}

		/*maintain the upper stream and lower stream pressure*/
		pneudrive.pSource.maintainPressure(upstreamPressure);

		/*maintain a downsteam pressure if necessary*/
	//	pneudrive.pSink.maintainPressure(downstreamPressure);

		pSink2.maintainPressure(downstreamPressure);

		tcur=millis();
		sinbase=sin(2*M_PI*fre*(tcur-t0)*0.001);
		sinbase=sinbase>=ratio?1:-1;
		externPd=amp*sinbase+offset;
		pneudrive.writePressure(0,externPd);
		pneudrive.writePressure(1,externPd);

		localCounter++;
		localCounter%=numUpdateValue;
		if(localCounter==0){
			totalNum++;
		}

		//Direct pump control without valve
		/*if(direction==1){
		pneudrive.pSource.pump.start();
		pneudrive.pSink.pump.stop();
			if(chamberp>=(amp+offset)){
				direction=-1;
			}
		}
		else if(direction==-1){
		pneudrive.pSource.pump.stop();
		pneudrive.pSink.pump.start();
			if(chamberp<=(-amp+offset)){
				direction=1;
				totalNum++;
			}
		}*/


	}
}

/*serial output using DMA*/
void serialDisplay() {
	//printf("Time: %d\r\n",millis());
	printf("%10lu %d  %d %d %d %d %d %d\r\n",
			millis(),
			(int)(pneudrive.pSource.pressure/1000),
			(int)(pneudrive.pSink.pressure/1000),
			(int)(externPd/1000),
//			(int)(amp/1000),
//			(int)(offset/1000),
			(int)(fre*10),
			(int)(pneudrive.chambers[0]->pressure/1000),
			(int)(length),
			totalNum
			);
}

//called when serial input receive string with ending '\r\n'
void serial3Callback(char *pSerialReceiveBuffer) {

	char commandChar;
	float pCommand = 0;

	//Read the received buffer with customized message. The first char means command type. The second number represents the interested chamber number. The third float number is the pressure command.
	sscanf(pSerialReceiveBuffer, "%c %f", &commandChar, &pCommand);


	if (commandChar == 'g') {
		loop_gogogo = 1;
	} else if (commandChar == 's') {
		loop_gogogo = 0;
		pneudrive.chambers[0]->writePressure(0);
	}
	else if(commandChar == 'a') {
		amp=pCommand;
	}
	else if(commandChar == 'f') {
			fre=pCommand;
			numUpdateValue=round(500/fre);
		}
	else if(commandChar == 'o') {
			offset=pCommand;
		}
	else if(commandChar == 'z') {
			pneudrive.chambers[0]->zeroPressure();
			pneudrive.pSource.zeroPressure();
			pneudrive.pSink.zeroPressure();
		}
	else if(commandChar == 'u') {
		upstreamPressure=pCommand;
		}
	else if(commandChar == 'd') {
		downstreamPressure=pCommand;
		}
	else if(commandChar == 'r') {
		ratio=pCommand;
		}

}

