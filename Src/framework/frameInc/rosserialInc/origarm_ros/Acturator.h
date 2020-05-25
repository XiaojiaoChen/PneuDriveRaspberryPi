#ifndef _ROS_origarm_ros_Acturator_h
#define _ROS_origarm_ros_Acturator_h

#include <rosserialInc/geometry_msgs/Pose.h>
#include <rosserialInc/ros/msg.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

namespace origarm_ros
{

  class Acturator : public ros::Msg
  {
    public:
      typedef int16_t _Pre_type;
      _Pre_type Pre;
      typedef int16_t _Dst_type;
      _Dst_type Dst;
      typedef geometry_msgs::Pose _Pose_type;
      _Pose_type Pose;

    Acturator():
      Pre(0),
      Dst(0),
      Pose()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_Pre;
      u_Pre.real = this->Pre;
      *(outbuffer + offset + 0) = (u_Pre.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_Pre.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->Pre);
      union {
        int16_t real;
        uint16_t base;
      } u_Dst;
      u_Dst.real = this->Dst;
      *(outbuffer + offset + 0) = (u_Dst.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_Dst.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->Dst);
      offset += this->Pose.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_Pre;
      u_Pre.base = 0;
      u_Pre.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_Pre.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->Pre = u_Pre.real;
      offset += sizeof(this->Pre);
      union {
        int16_t real;
        uint16_t base;
      } u_Dst;
      u_Dst.base = 0;
      u_Dst.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_Dst.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->Dst = u_Dst.real;
      offset += sizeof(this->Dst);
      offset += this->Pose.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "origarm_ros/Acturator"; };
    const char * getMD5(){ return "85fd28494562c91d8c35c06eb91d5cb0"; };

  };

}
#endif
