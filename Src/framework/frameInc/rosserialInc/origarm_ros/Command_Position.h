#ifndef _ROS_origarm_ros_Command_Position_h
#define _ROS_origarm_ros_Command_Position_h

#include <rosserialInc/geometry_msgs/Pose.h>
#include <rosserialInc/ros/msg.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

namespace origarm_ros
{

  class Command_Position : public ros::Msg
  {
    public:
      typedef geometry_msgs::Pose _pose_type;
      _pose_type pose;

    Command_Position():
      pose()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->pose.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->pose.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "origarm_ros/Command_Position"; };
    const char * getMD5(){ return "f192399f711a48924df9a394d37edd67"; };

  };

}
#endif
