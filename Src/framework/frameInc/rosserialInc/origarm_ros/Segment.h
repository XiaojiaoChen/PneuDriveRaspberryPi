#ifndef _ROS_origarm_ros_Segment_h
#define _ROS_origarm_ros_Segment_h

#include <rosserialInc/origarm_ros/Acturator.h>
#include <rosserialInc/ros/msg.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

namespace origarm_ros
{

  class Segment : public ros::Msg
  {
    public:
      origarm_ros::Acturator acturator[6];

    Segment():
      acturator()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      for( uint32_t i = 0; i < 6; i++){
      offset += this->acturator[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      for( uint32_t i = 0; i < 6; i++){
      offset += this->acturator[i].deserialize(inbuffer + offset);
      }
     return offset;
    }

    const char * getType(){ return "origarm_ros/Segment"; };
    const char * getMD5(){ return "134a16d976fdcef16e62dbf0c0b89e4d"; };

  };

}
#endif
