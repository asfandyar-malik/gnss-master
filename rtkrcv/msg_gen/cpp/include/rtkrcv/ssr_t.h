/* Auto-generated by genmsg_cpp for file /home/asfandyar/gnss-master/rtkrcv/msg/ssr_t.msg */
#ifndef RTKRCV_MESSAGE_SSR_T_H
#define RTKRCV_MESSAGE_SSR_T_H
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
struct ssr_t_ {
  typedef ssr_t_<ContainerAllocator> Type;

  ssr_t_()
  : t0()
  , udi()
  , iod()
  , iode(0)
  , ura(0)
  , refd(0)
  , deph()
  , ddeph()
  , dclk()
  , hrclk(0.0)
  , cbias()
  , update(0)
  {
  }

  ssr_t_(const ContainerAllocator& _alloc)
  : t0()
  , udi(_alloc)
  , iod(_alloc)
  , iode(0)
  , ura(0)
  , refd(0)
  , deph(_alloc)
  , ddeph(_alloc)
  , dclk(_alloc)
  , hrclk(0.0)
  , cbias(_alloc)
  , update(0)
  {
  }

  typedef ros::Time _t0_type;
  ros::Time t0;

  typedef std::vector<double, typename ContainerAllocator::template rebind<double>::other >  _udi_type;
  std::vector<double, typename ContainerAllocator::template rebind<double>::other >  udi;

  typedef std::vector<int32_t, typename ContainerAllocator::template rebind<int32_t>::other >  _iod_type;
  std::vector<int32_t, typename ContainerAllocator::template rebind<int32_t>::other >  iod;

  typedef int32_t _iode_type;
  int32_t iode;

  typedef int32_t _ura_type;
  int32_t ura;

  typedef int32_t _refd_type;
  int32_t refd;

  typedef std::vector<double, typename ContainerAllocator::template rebind<double>::other >  _deph_type;
  std::vector<double, typename ContainerAllocator::template rebind<double>::other >  deph;

  typedef std::vector<double, typename ContainerAllocator::template rebind<double>::other >  _ddeph_type;
  std::vector<double, typename ContainerAllocator::template rebind<double>::other >  ddeph;

  typedef std::vector<double, typename ContainerAllocator::template rebind<double>::other >  _dclk_type;
  std::vector<double, typename ContainerAllocator::template rebind<double>::other >  dclk;

  typedef double _hrclk_type;
  double hrclk;

  typedef std::vector<float, typename ContainerAllocator::template rebind<float>::other >  _cbias_type;
  std::vector<float, typename ContainerAllocator::template rebind<float>::other >  cbias;

  typedef uint8_t _update_type;
  uint8_t update;


  typedef boost::shared_ptr< ::rtkrcv::ssr_t_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::rtkrcv::ssr_t_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct ssr_t
typedef  ::rtkrcv::ssr_t_<std::allocator<void> > ssr_t;

typedef boost::shared_ptr< ::rtkrcv::ssr_t> ssr_tPtr;
typedef boost::shared_ptr< ::rtkrcv::ssr_t const> ssr_tConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::rtkrcv::ssr_t_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::rtkrcv::ssr_t_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace rtkrcv

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::rtkrcv::ssr_t_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::rtkrcv::ssr_t_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::rtkrcv::ssr_t_<ContainerAllocator> > {
  static const char* value() 
  {
    return "7f15313cb606c32dfc95c29506d2fce0";
  }

  static const char* value(const  ::rtkrcv::ssr_t_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x7f15313cb606c32dULL;
  static const uint64_t static_value2 = 0xfc95c29506d2fce0ULL;
};

template<class ContainerAllocator>
struct DataType< ::rtkrcv::ssr_t_<ContainerAllocator> > {
  static const char* value() 
  {
    return "rtkrcv/ssr_t";
  }

  static const char* value(const  ::rtkrcv::ssr_t_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::rtkrcv::ssr_t_<ContainerAllocator> > {
  static const char* value() 
  {
    return " # SSR correction type\n\
\n\
time t0             # epoch time (GPST)\n\
float64[] udi       # SSR update interval (s)\n\
int32[] iod\n\
int32 iode          # issue of data\n\
int32 ura           # URA indicator\n\
int32 refd          # sat ref datum (0:ITRF,1:regional)\n\
float64[] deph      # delta orbit {radial,along,cross} (m)\n\
float64[] ddeph     # dot delta orbit {radial,along,cross} (m/s)\n\
float64[] dclk      # delta clock {c0,c1,c2} (m,m/s,m/s^2)\n\
float64 hrclk       # high-rate clock corection (m)\n\
float32[] cbias     # code biases (m)\n\
uint8 update        # update flag (0:no update,1:update)\n\
";
  }

  static const char* value(const  ::rtkrcv::ssr_t_<ContainerAllocator> &) { return value(); } 
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::rtkrcv::ssr_t_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.t0);
    stream.next(m.udi);
    stream.next(m.iod);
    stream.next(m.iode);
    stream.next(m.ura);
    stream.next(m.refd);
    stream.next(m.deph);
    stream.next(m.ddeph);
    stream.next(m.dclk);
    stream.next(m.hrclk);
    stream.next(m.cbias);
    stream.next(m.update);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct ssr_t_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::rtkrcv::ssr_t_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::rtkrcv::ssr_t_<ContainerAllocator> & v) 
  {
    s << indent << "t0: ";
    Printer<ros::Time>::stream(s, indent + "  ", v.t0);
    s << indent << "udi[]" << std::endl;
    for (size_t i = 0; i < v.udi.size(); ++i)
    {
      s << indent << "  udi[" << i << "]: ";
      Printer<double>::stream(s, indent + "  ", v.udi[i]);
    }
    s << indent << "iod[]" << std::endl;
    for (size_t i = 0; i < v.iod.size(); ++i)
    {
      s << indent << "  iod[" << i << "]: ";
      Printer<int32_t>::stream(s, indent + "  ", v.iod[i]);
    }
    s << indent << "iode: ";
    Printer<int32_t>::stream(s, indent + "  ", v.iode);
    s << indent << "ura: ";
    Printer<int32_t>::stream(s, indent + "  ", v.ura);
    s << indent << "refd: ";
    Printer<int32_t>::stream(s, indent + "  ", v.refd);
    s << indent << "deph[]" << std::endl;
    for (size_t i = 0; i < v.deph.size(); ++i)
    {
      s << indent << "  deph[" << i << "]: ";
      Printer<double>::stream(s, indent + "  ", v.deph[i]);
    }
    s << indent << "ddeph[]" << std::endl;
    for (size_t i = 0; i < v.ddeph.size(); ++i)
    {
      s << indent << "  ddeph[" << i << "]: ";
      Printer<double>::stream(s, indent + "  ", v.ddeph[i]);
    }
    s << indent << "dclk[]" << std::endl;
    for (size_t i = 0; i < v.dclk.size(); ++i)
    {
      s << indent << "  dclk[" << i << "]: ";
      Printer<double>::stream(s, indent + "  ", v.dclk[i]);
    }
    s << indent << "hrclk: ";
    Printer<double>::stream(s, indent + "  ", v.hrclk);
    s << indent << "cbias[]" << std::endl;
    for (size_t i = 0; i < v.cbias.size(); ++i)
    {
      s << indent << "  cbias[" << i << "]: ";
      Printer<float>::stream(s, indent + "  ", v.cbias[i]);
    }
    s << indent << "update: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.update);
  }
};


} // namespace message_operations
} // namespace ros

#endif // RTKRCV_MESSAGE_SSR_T_H
