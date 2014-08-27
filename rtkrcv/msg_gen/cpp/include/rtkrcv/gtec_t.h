/* Auto-generated by genmsg_cpp for file /home/asfandyar/gnss-master/rtkrcv/msg/gtec_t.msg */
#ifndef RTKRCV_MESSAGE_GTEC_T_H
#define RTKRCV_MESSAGE_GTEC_T_H
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
struct gtec_t_ {
  typedef gtec_t_<ContainerAllocator> Type;

  gtec_t_()
  : time()
  , sat(0)
  , ntype(0)
  , ndata(0)
  , data()
  {
  }

  gtec_t_(const ContainerAllocator& _alloc)
  : time()
  , sat(0)
  , ntype(0)
  , ndata(0)
  , data(_alloc)
  {
  }

  typedef ros::Time _time_type;
  ros::Time time;

  typedef int32_t _sat_type;
  int32_t sat;

  typedef int32_t _ntype_type;
  int32_t ntype;

  typedef int32_t _ndata_type;
  int32_t ndata;

  typedef std::vector<float, typename ContainerAllocator::template rebind<float>::other >  _data_type;
  std::vector<float, typename ContainerAllocator::template rebind<float>::other >  data;


  typedef boost::shared_ptr< ::rtkrcv::gtec_t_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::rtkrcv::gtec_t_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct gtec_t
typedef  ::rtkrcv::gtec_t_<std::allocator<void> > gtec_t;

typedef boost::shared_ptr< ::rtkrcv::gtec_t> gtec_tPtr;
typedef boost::shared_ptr< ::rtkrcv::gtec_t const> gtec_tConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::rtkrcv::gtec_t_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::rtkrcv::gtec_t_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace rtkrcv

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::rtkrcv::gtec_t_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::rtkrcv::gtec_t_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::rtkrcv::gtec_t_<ContainerAllocator> > {
  static const char* value() 
  {
    return "dc06d9d4d6785861758554907a129abc";
  }

  static const char* value(const  ::rtkrcv::gtec_t_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0xdc06d9d4d6785861ULL;
  static const uint64_t static_value2 = 0x758554907a129abcULL;
};

template<class ContainerAllocator>
struct DataType< ::rtkrcv::gtec_t_<ContainerAllocator> > {
  static const char* value() 
  {
    return "rtkrcv/gtec_t";
  }

  static const char* value(const  ::rtkrcv::gtec_t_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::rtkrcv::gtec_t_<ContainerAllocator> > {
  static const char* value() 
  {
    return "# G_TEC type\n\
\n\
time time              # epoch time (GPST)\n\
\n\
int32 sat              # satellite number\n\
int32 ntype            # number of stec data types\n\
int32 ndata            # number of stec data\n\
\n\
float32[] data         # G_TEC data\n\
";
  }

  static const char* value(const  ::rtkrcv::gtec_t_<ContainerAllocator> &) { return value(); } 
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::rtkrcv::gtec_t_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.time);
    stream.next(m.sat);
    stream.next(m.ntype);
    stream.next(m.ndata);
    stream.next(m.data);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct gtec_t_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::rtkrcv::gtec_t_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::rtkrcv::gtec_t_<ContainerAllocator> & v) 
  {
    s << indent << "time: ";
    Printer<ros::Time>::stream(s, indent + "  ", v.time);
    s << indent << "sat: ";
    Printer<int32_t>::stream(s, indent + "  ", v.sat);
    s << indent << "ntype: ";
    Printer<int32_t>::stream(s, indent + "  ", v.ntype);
    s << indent << "ndata: ";
    Printer<int32_t>::stream(s, indent + "  ", v.ndata);
    s << indent << "data[]" << std::endl;
    for (size_t i = 0; i < v.data.size(); ++i)
    {
      s << indent << "  data[" << i << "]: ";
      Printer<float>::stream(s, indent + "  ", v.data[i]);
    }
  }
};


} // namespace message_operations
} // namespace ros

#endif // RTKRCV_MESSAGE_GTEC_T_H
