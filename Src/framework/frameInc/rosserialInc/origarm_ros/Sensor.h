#ifndef _ROS_origarm_ros_Sensor_h
#define _ROS_origarm_ros_Sensor_h

#include <rosserialInc/origarm_ros/Segment.h>
#include <rosserialInc/ros/msg.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

namespace origarm_ros
{

  class Sensor : public ros::Msg
  {
    public:
      uint32_t segment_length;
      typedef origarm_ros::Segment _segment_type;
      _segment_type st_segment;
      _segment_type * segment;

    Sensor():
      segment_length(0), segment(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->segment_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->segment_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->segment_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->segment_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->segment_length);
      for( uint32_t i = 0; i < segment_length; i++){
      offset += this->segment[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t segment_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      segment_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      segment_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      segment_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->segment_length);
      if(segment_lengthT > segment_length)
        this->segment = (origarm_ros::Segment*)realloc(this->segment, segment_lengthT * sizeof(origarm_ros::Segment));
      segment_length = segment_lengthT;
      for( uint32_t i = 0; i < segment_length; i++){
      offset += this->st_segment.deserialize(inbuffer + offset);
        memcpy( &(this->segment[i]), &(this->st_segment), sizeof(origarm_ros::Segment));
      }
     return offset;
    }

    const char * getType(){ return "origarm_ros/Sensor"; };
    const char * getMD5(){ return "b37aa7aee4727db4e148da671e5470ea"; };

  };

}
#endif
