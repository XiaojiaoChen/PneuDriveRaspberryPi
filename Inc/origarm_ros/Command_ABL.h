#ifndef _ROS_origarm_ros_Command_ABL_h
#define _ROS_origarm_ros_Command_ABL_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "origarm_ros/Seg_ABL.h"

namespace origarm_ros
{

  class Command_ABL : public ros::Msg
  {
    public:
      origarm_ros::Seg_ABL segment[1];

    Command_ABL():
      segment()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      for( uint32_t i = 0; i < 1; i++){
      offset += this->segment[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      for( uint32_t i = 0; i < 1; i++){
      offset += this->segment[i].deserialize(inbuffer + offset);
      }
     return offset;
    }

    const char * getType(){ return "origarm_ros/Command_ABL"; };
    const char * getMD5(){ return "78385bb25f040e30c47d0755743d0d44"; };

  };

}
#endif
