/*
 * ROSSerialNode.cpp
 *
 *  Created on: May 19, 2020
 *      Author: Xiaojiao Chen
 *       Email: yii402072495@gmail.com
 */

#include <rosserialInc/std_msgs/UInt16.h>
#include <ROSSerialNode.h>
#include "myUsartFunction.h"

ros::NodeHandle rosSerialNode;

/***************  Modify 1 begin***********************/
std_msgs::UInt16 pubData;
ros::Publisher _pub("pubTopicName", &pubData);

void rosNodeSubCB(const std_msgs::UInt16& msg);
ros::Subscriber<std_msgs::UInt16> _sub("subTopicName", &rosNodeSubCB);
/***************  Modify 1 end***********************/

void rosNodeSetup(){
	rosSerialNode.initNode();

	/***************Modify 2 begin **************/
	rosSerialNode.advertise(_pub);
	rosSerialNode.subscribe(_sub);
	/***************Modify 2 end  **************/
}


void rosNodeSpinOnce(void)
{
   rosSerialNode.spinOnce();
}

/***************Modify 3 begin  **************/
void rosNodePub(){
	pubData.data=2;
	_pub.publish(&pubData);
}

void rosNodeSubCB(const std_msgs::UInt16& msg){
	printf("Receive test topi %d\r\n",msg.data);
}

/***************Modify 3  end **************/
