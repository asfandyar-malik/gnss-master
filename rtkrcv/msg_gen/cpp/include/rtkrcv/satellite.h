/* Auto-generated by genmsg_cpp for file /home/asfandyar/gnss-master/rtkrcv/msg/satellite.msg */
#ifndef RTKRCV_MESSAGE_SATELLITE_H
#define RTKRCV_MESSAGE_SATELLITE_H
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


namespace rtkrcv
{
template <class ContainerAllocator>
struct satellite_ {
  typedef satellite_<ContainerAllocator> Type;

  satellite_()
  : id(0)
  , vs(0)
  , elevation(0.0)
  , asimuth(0.0)
  {
  }

  satellite_(const ContainerAllocator& _alloc)
  : id(0)
  , vs(0)
  , elevation(0.0)
  , asimuth(0.0)
  {
  }

  typedef int32_t _id_type;
  int32_t id;

  typedef int32_t _vs_type;
  int32_t vs;

  typedef double _elevation_type;
  double elevation;

  typedef double _asimuth_type;
  double asimuth;


  typedef boost::shared_ptr< ::rtkrcv::satellite_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::rtkrcv::satellite_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct satellite
typedef  ::rtkrcv::satellite_<std::allocator<void> > satellite;

typedef boost::shared_ptr< ::rtkrcv::satellite> satellitePtr;
typedef boost::shared_ptr< ::rtkrcv::satellite const> satelliteConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::rtkrcv::satellite_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::rtkrcv::satellite_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace rtkrcv

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::rtkrcv::satellite_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::rtkrcv::satellite_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::rtkrcv::satellite_<ContainerAllocator> > {
  static const char* value() 
  {
    return "a188f62faa6709917125d14ff7ffc49a";
  }

  static const char* value(const  ::rtkrcv::satellite_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0xa188f62faa670991ULL;
  static const uint64_t static_value2 = 0x7125d14ff7ffc49aULL;
};

template<class ContainerAllocator>
struct DataType< ::rtkrcv::satellite_<ContainerAllocator> > {
  static const char* value() 
  {
    return "rtkrcv/satellite";
  }

  static const char* value(const  ::rtkrcv::satellite_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::rtkrcv::satellite_<ContainerAllocator> > {
  static const char* value() 
  {
    return "#Characteristics of on one satellite   \n\
\n\
  int32 id\n\
  int32 vs\n\
  float64 elevation\n\
  float64 asimuth\n\
";
  }

  static const char* value(const  ::rtkrcv::satellite_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct IsFixedSize< ::rtkrcv::satellite_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::rtkrcv::satellite_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.id);
    stream.next(m.vs);
    stream.next(m.elevation);
    stream.next(m.asimuth);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct satellite_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::rtkrcv::satellite_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::rtkrcv::satellite_<ContainerAllocator> & v) 
  {
    s << indent << "id: ";
    Printer<int32_t>::stream(s, indent + "  ", v.id);
    s << indent << "vs: ";
    Printer<int32_t>::stream(s, indent + "  ", v.vs);
    s << indent << "elevation: ";
    Printer<double>::stream(s, indent + "  ", v.elevation);
    s << indent << "asimuth: ";
    Printer<double>::stream(s, indent + "  ", v.asimuth);
  }
};


} // namespace message_operations
} // namespace ros

#endif // RTKRCV_MESSAGE_SATELLITE_H

