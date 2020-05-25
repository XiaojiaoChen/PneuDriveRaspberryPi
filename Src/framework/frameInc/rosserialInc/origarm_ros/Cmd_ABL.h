#ifndef _ROS_origarm_ros_Cmd_ABL_h
#define _ROS_origarm_ros_Cmd_ABL_h

#include <rosserialInc/origarm_ros/Seg_ABL.h>
#include <rosserialInc/ros/msg.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

namespace origarm_ros
{

  class Cmd_ABL : public ros::Msg
  {
    public:
      origarm_ros::Seg_ABL segment[9];
      typedef int16_t _segmentNumber_type;
      _segmentNumber_type segmentNumber;

    Cmd_ABL():
      segment(),
      segmentNumber(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      for( uint32_t i = 0; i < 9; i++){
      offset += this->segment[i].serialize(outbuffer + offset);
      }
      union {
        int16_t real;
        uint16_t base;
      } u_segmentNumber;
      u_segmentNumber.real = this->segmentNumber;
      *(outbuffer + offset + 0) = (u_segmentNumber.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_segmentNumber.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->segmentNumber);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      for( uint32_t i = 0; i < 9; i++){
      offset += this->segment[i].deserialize(inbuffer + offset);
      }
      union {
        int16_t real;
        uint16_t base;
      } u_segmentNumber;
      u_segmentNumber.base = 0;
      u_segmentNumber.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_segmentNumber.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->segmentNumber = u_segmentNumber.real;
      offset += sizeof(this->segmentNumber);
     return offset;
    }

    const char * getType(){ return "origarm_ros/Cmd_ABL"; };
    const char * getMD5(){ return "efd81899c21125740aa558b9cc2985a2"; };

  };

}
#endif
