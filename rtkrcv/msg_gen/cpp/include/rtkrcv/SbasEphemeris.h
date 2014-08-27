/* Auto-generated by genmsg_cpp for file /home/asfandyar/gnss-master/rtkrcv/msg/SbasEphemeris.msg */
#ifndef RTKRCV_MESSAGE_SBASEPHEMERIS_H
#define RTKRCV_MESSAGE_SBASEPHEMERIS_H
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/time.h"

#include "ros/macros.h"

#include "ros/assert.h"

#include "geometry_msgs/Vector3.h"
#include "geometry_msgs/Vector3.h"
#include "geometry_msgs/Vector3.h"

namespace rtkrcv
{
template <class ContainerAllocator>
struct SbasEphemeris_ {
  typedef SbasEphemeris_<ContainerAllocator> Type;

  SbasEphemeris_()
  : sat(0)
  , t0()
  , tof()
  , sva(0)
  , svh(0)
  , pos()
  , vel()
  , acc()
  , af0(0.0)
  , af1(0.0)
  {
  }

  SbasEphemeris_(const ContainerAllocator& _alloc)
  : sat(0)
  , t0()
  , tof()
  , sva(0)
  , svh(0)
  , pos(_alloc)
  , vel(_alloc)
  , acc(_alloc)
  , af0(0.0)
  , af1(0.0)
  {
  }

  typedef int32_t _sat_type;
  int32_t sat;

  typedef ros::Time _t0_type;
  ros::Time t0;

  typedef ros::Time _tof_type;
  ros::Time tof;

  typedef int32_t _sva_type;
  int32_t sva;

  typedef int32_t _svh_type;
  int32_t svh;

  typedef  ::geometry_msgs::Vector3_<ContainerAllocator>  _pos_type;
   ::geometry_msgs::Vector3_<ContainerAllocator>  pos;

  typedef  ::geometry_msgs::Vector3_<ContainerAllocator>  _vel_type;
   ::geometry_msgs::Vector3_<ContainerAllocator>  vel;

  typedef  ::geometry_msgs::Vector3_<ContainerAllocator>  _acc_type;
   ::geometry_msgs::Vector3_<ContainerAllocator>  acc;

  typedef double _af0_type;
  double af0;

  typedef double _af1_type;
  double af1;


  typedef boost::shared_ptr< ::rtkrcv::SbasEphemeris_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::rtkrcv::SbasEphemeris_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct SbasEphemeris
typedef  ::rtkrcv::SbasEphemeris_<std::allocator<void> > SbasEphemeris;

typedef boost::shared_ptr< ::rtkrcv::SbasEphemeris> SbasEphemerisPtr;
typedef boost::shared_ptr< ::rtkrcv::SbasEphemeris const> SbasEphemerisConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::rtkrcv::SbasEphemeris_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::rtkrcv::SbasEphemeris_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace rtkrcv

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::rtkrcv::SbasEphemeris_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::rtkrcv::SbasEphemeris_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::rtkrcv::SbasEphemeris_<ContainerAllocator> > {
  static const char* value() 
  {
    return "40f5d361177a2ebb573aec7e53721180";
  }

  static const char* value(const  ::rtkrcv::SbasEphemeris_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x40f5d361177a2ebbULL;
  static const uint64_t static_value2 = 0x573aec7e53721180ULL;
};

template<class ContainerAllocator>
struct DataType< ::rtkrcv::SbasEphemeris_<ContainerAllocator> > {
  static const char* value() 
  {
    return "rtkrcv/SbasEphemeris";
  }

  static const char* value(const  ::rtkrcv::SbasEphemeris_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::rtkrcv::SbasEphemeris_<ContainerAllocator> > {
  static const char* value() 
  {
    return "# SBAS ephemeris type\n\
\n\
int32 sat                       # satellite number\n\
time t0                         # reference epoch time (GPST)\n\
time tof                        # time of message frame (GPST)\n\
int32 sva                       # SV accuracy (URA index)\n\
int32 svh                       # SV health (0:ok)\n\
\n\
geometry_msgs/Vector3 pos       # satellite position (m) (ecef)\n\
geometry_msgs/Vector3 vel       # satellite velocity (m/s) (ecef)\n\
geometry_msgs/Vector3 acc       # satellite acceleration (m/s^2) (ecef)\n\
float64 af0                     # satellite clock-offset (s,s/s)\n\
float64 af1                     # satellite drift (s,s/s)\n\
================================================================================\n\
MSG: geometry_msgs/Vector3\n\
# This represents a vector in free space. \n\
\n\
float64 x\n\
float64 y\n\
float64 z\n\
";
  }

  static const char* value(const  ::rtkrcv::SbasEphemeris_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct IsFixedSize< ::rtkrcv::SbasEphemeris_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::rtkrcv::SbasEphemeris_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.sat);
    stream.next(m.t0);
    stream.next(m.tof);
    stream.next(m.sva);
    stream.next(m.svh);
    stream.next(m.pos);
    stream.next(m.vel);
    stream.next(m.acc);
    stream.next(m.af0);
    stream.next(m.af1);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct SbasEphemeris_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::rtkrcv::SbasEphemeris_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::rtkrcv::SbasEphemeris_<ContainerAllocator> & v) 
  {
    s << indent << "sat: ";
    Printer<int32_t>::stream(s, indent + "  ", v.sat);
    s << indent << "t0: ";
    Printer<ros::Time>::stream(s, indent + "  ", v.t0);
    s << indent << "tof: ";
    Printer<ros::Time>::stream(s, indent + "  ", v.tof);
    s << indent << "sva: ";
    Printer<int32_t>::stream(s, indent + "  ", v.sva);
    s << indent << "svh: ";
    Printer<int32_t>::stream(s, indent + "  ", v.svh);
    s << indent << "pos: ";
s << std::endl;
    Printer< ::geometry_msgs::Vector3_<ContainerAllocator> >::stream(s, indent + "  ", v.pos);
    s << indent << "vel: ";
s << std::endl;
    Printer< ::geometry_msgs::Vector3_<ContainerAllocator> >::stream(s, indent + "  ", v.vel);
    s << indent << "acc: ";
s << std::endl;
    Printer< ::geometry_msgs::Vector3_<ContainerAllocator> >::stream(s, indent + "  ", v.acc);
    s << indent << "af0: ";
    Printer<double>::stream(s, indent + "  ", v.af0);
    s << indent << "af1: ";
    Printer<double>::stream(s, indent + "  ", v.af1);
  }
};


} // namespace message_operations
} // namespace ros

#endif // RTKRCV_MESSAGE_SBASEPHEMERIS_H

