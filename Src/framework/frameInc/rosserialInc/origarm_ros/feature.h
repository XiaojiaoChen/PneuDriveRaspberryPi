#ifndef _ROS_SERVICE_feature_h
#define _ROS_SERVICE_feature_h
#include <rosserialInc/ros/msg.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

namespace origarm_ros
{

static const char FEATURE[] = "origarm_ros/feature";

  class featureRequest : public ros::Msg
  {
    public:

    featureRequest()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
     return offset;
    }

    const char * getType(){ return FEATURE; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class featureResponse : public ros::Msg
  {
    public:

    featureResponse()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
     return offset;
    }

    const char * getType(){ return FEATURE; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class feature {
    public:
    typedef featureRequest Request;
    typedef featureResponse Response;
  };

}
#endif
