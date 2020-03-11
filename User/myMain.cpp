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

void setup()
{
	/*setup 1000Hz control loop*/
	setPeriodControlLoop(50);

	/*setup 50Hz serial display loop*/
	setPeriodSendLoop(50);

	/*soft arm chambers' PWM port mapping*/
	softArm.setupChamberPWMPort();

	/*start canBus receive*/
	canConfig();

	/**********start the SPI slave in DMA*****/
	spiSlaveStart();

}

void loop()
{

	/**********The sensor feedback are automatically received from nodes by CAN,
	 and stored in sensorData[segNum][bellowNum] in softArm.*/

	/**********The commands are automatically updated from SPI1 using DMA,
	 and stored in commandData[segNum][bellowNum] in softArm.*/

	/**Write the command of each chamber, either pressure or opening type*/
	softArm.writeCommandAll();


}



/*serial output using DMA*/
void serialDisplay()
{
	printf("ahh\r\n");
}


