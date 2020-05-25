#ifndef _ROS_origarm_ros_Seg_Pre_h
#define _ROS_origarm_ros_Seg_Pre_h

#include <rosserialInc/origarm_ros/Valve.h>
#include <rosserialInc/ros/msg.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

namespace origarm_ros
{

  class Seg_Pre : public ros::Msg
  {
    public:
      origarm_ros::Valve command[6];

    Seg_Pre():
      command()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      for( uint32_t i = 0; i < 6; i++){
      offset += this->command[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      for( uint32_t i = 0; i < 6; i++){
      offset += this->command[i].deserialize(inbuffer + offset);
      }
     return offset;
    }

    const char * getType(){ return "origarm_ros/Seg_Pre"; };
    const char * getMD5(){ return "dcfa8f034f96384a0cbadd0dcc436399"; };

  };

}
#endif
