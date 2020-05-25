/*
 * ROSSerialNode.h
 *
 *  Created on: May 20, 2020
 *      Author: Xiaojiao Chen
 *       Email: yii402072495@gmail.com
 */

#ifndef ROSSERIALNODE_H_
#define ROSSERIALNODE_H_

#include "stm32f7xx_hal.h"
#include "ros.h"


void rosNodeSetup();

void rosNodeSpinOnce(void);

void rosNodePub();
#endif /* ROSSERIALNODE_H_ */
