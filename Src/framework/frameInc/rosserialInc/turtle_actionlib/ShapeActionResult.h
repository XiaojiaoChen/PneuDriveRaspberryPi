#ifndef _ROS_turtle_actionlib_ShapeActionResult_h
#define _ROS_turtle_actionlib_ShapeActionResult_h

#include <rosserialInc/actionlib_msgs/GoalStatus.h>
#include <rosserialInc/ros/msg.h>
#include <rosserialInc/std_msgs/Header.h>
#include <rosserialInc/turtle_actionlib/ShapeResult.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

namespace turtle_actionlib
{

  class ShapeActionResult : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef actionlib_msgs::GoalStatus _status_type;
      _status_type status;
      typedef turtle_actionlib::ShapeResult _result_type;
      _result_type result;

    ShapeActionResult():
      header(),
      status(),
      result()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->status.serialize(outbuffer + offset);
      offset += this->result.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->status.deserialize(inbuffer + offset);
      offset += this->result.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "turtle_actionlib/ShapeActionResult"; };
    const char * getMD5(){ return "c8d13d5d140f1047a2e4d3bf5c045822"; };

  };

}
#endif
