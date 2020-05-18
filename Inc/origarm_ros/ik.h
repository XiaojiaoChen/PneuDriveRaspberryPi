#ifndef _ROS_SERVICE_ik_h
#define _ROS_SERVICE_ik_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "origarm_ros/States.h"
#include "origarm_ros/Command_ABL.h"

namespace origarm_ros
{

static const char IK[] = "origarm_ros/ik";

  class ikRequest : public ros::Msg
  {
    public:
      typedef origarm_ros::States _input_type;
      _input_type input;

    ikRequest():
      input()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->input.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->input.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return IK; };
    const char * getMD5(){ return "7bac1c91ea3f5a61397121d5a1c5a894"; };

  };

  class ikResponse : public ros::Msg
  {
    public:
      typedef origarm_ros::Command_ABL _output_type;
      _output_type output;

    ikResponse():
      output()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->output.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->output.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return IK; };
    const char * getMD5(){ return "33a9a17e2cee799efabd88609880be0d"; };

  };

  class ik {
    public:
    typedef ikRequest Request;
    typedef ikResponse Response;
  };

}
#endif
