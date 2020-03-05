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
#include <SoftArm.h>



SOFT_ARM softArm;

void setup()
{
	/* 1000Hz control loop*/
	setPeriodControlLoop(50);

	/* 50Hz serial display loop*/
	setPeriodSendLoop(50);

	softArm.setup();

}

void loop()
{
	softArm.loop();

}

void serialDisplay()
{

}


