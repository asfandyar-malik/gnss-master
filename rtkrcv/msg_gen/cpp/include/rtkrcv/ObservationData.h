/* Auto-generated by genmsg_cpp for file /home/asfandyar/gnss-master/rtkrcv/msg/ObservationData.msg */
#ifndef RTKRCV_MESSAGE_OBSERVATIONDATA_H
#define RTKRCV_MESSAGE_OBSERVATIONDATA_H
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
struct ObservationData_ {
  typedef ObservationData_<ContainerAllocator> Type;

  ObservationData_()
  : time()
  , sat(0)
  , rcv(0)
  , SNR()
  , LLI()
  , code()
  , L()
  , P()
  , D()
  {
  }

  ObservationData_(const ContainerAllocator& _alloc)
  : time()
  , sat(0)
  , rcv(0)
  , SNR(_alloc)
  , LLI(_alloc)
  , code(_alloc)
  , L(_alloc)
  , P(_alloc)
  , D(_alloc)
  {
  }

  typedef ros::Time _time_type;
  ros::Time time;

  typedef uint8_t _sat_type;
  uint8_t sat;

  typedef uint8_t _rcv_type;
  uint8_t rcv;

  typedef std::vector<uint8_t, typename ContainerAllocator::template rebind<uint8_t>::other >  _SNR_type;
  std::vector<uint8_t, typename ContainerAllocator::template rebind<uint8_t>::other >  SNR;

  typedef std::vector<uint8_t, typename ContainerAllocator::template rebind<uint8_t>::other >  _LLI_type;
  std::vector<uint8_t, typename ContainerAllocator::template rebind<uint8_t>::other >  LLI;

  typedef std::vector<uint8_t, typename ContainerAllocator::template rebind<uint8_t>::other >  _code_type;
  std::vector<uint8_t, typename ContainerAllocator::template rebind<uint8_t>::other >  code;

  typedef std::vector<double, typename ContainerAllocator::template rebind<double>::other >  _L_type;
  std::vector<double, typename ContainerAllocator::template rebind<double>::other >  L;

  typedef std::vector<double, typename ContainerAllocator::template rebind<double>::other >  _P_type;
  std::vector<double, typename ContainerAllocator::template rebind<double>::other >  P;

  typedef std::vector<float, typename ContainerAllocator::template rebind<float>::other >  _D_type;
  std::vector<float, typename ContainerAllocator::template rebind<float>::other >  D;


  typedef boost::shared_ptr< ::rtkrcv::ObservationData_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::rtkrcv::ObservationData_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct ObservationData
typedef  ::rtkrcv::ObservationData_<std::allocator<void> > ObservationData;

typedef boost::shared_ptr< ::rtkrcv::ObservationData> ObservationDataPtr;
typedef boost::shared_ptr< ::rtkrcv::ObservationData const> ObservationDataConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::rtkrcv::ObservationData_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::rtkrcv::ObservationData_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace rtkrcv

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::rtkrcv::ObservationData_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::rtkrcv::ObservationData_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::rtkrcv::ObservationData_<ContainerAllocator> > {
  static const char* value() 
  {
    return "2414e19a8314b63733505d032ad576fc";
  }

  static const char* value(const  ::rtkrcv::ObservationData_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x2414e19a8314b637ULL;
  static const uint64_t static_value2 = 0x33505d032ad576fcULL;
};

template<class ContainerAllocator>
struct DataType< ::rtkrcv::ObservationData_<ContainerAllocator> > {
  static const char* value() 
  {
    return "rtkrcv/ObservationData";
  }

  static const char* value(const  ::rtkrcv::ObservationData_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::rtkrcv::ObservationData_<ContainerAllocator> > {
  static const char* value() 
  {
    return " # observation data records\n\
\n\
time time             # receiver sampling time (GPST)\n\
uint8 sat             # satellite\n\
uint8 rcv             # receiver number\n\
uint8[] SNR           # signal strength (0.25 dBHz)\n\
uint8[] LLI           # loss of lock indicator\n\
uint8[] code          # code indicator (CODE_???)\n\
float64[] L           # observation data carrier-phase (cycle)\n\
float64[] P           # observation data pseudorange (m)\n\
float32[] D           # observation data doppler frequency (Hz)\n\
\n\
";
  }

  static const char* value(const  ::rtkrcv::ObservationData_<ContainerAllocator> &) { return value(); } 
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::rtkrcv::ObservationData_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.time);
    stream.next(m.sat);
    stream.next(m.rcv);
    stream.next(m.SNR);
    stream.next(m.LLI);
    stream.next(m.code);
    stream.next(m.L);
    stream.next(m.P);
    stream.next(m.D);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct ObservationData_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::rtkrcv::ObservationData_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::rtkrcv::ObservationData_<ContainerAllocator> & v) 
  {
    s << indent << "time: ";
    Printer<ros::Time>::stream(s, indent + "  ", v.time);
    s << indent << "sat: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.sat);
    s << indent << "rcv: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.rcv);
    s << indent << "SNR[]" << std::endl;
    for (size_t i = 0; i < v.SNR.size(); ++i)
    {
      s << indent << "  SNR[" << i << "]: ";
      Printer<uint8_t>::stream(s, indent + "  ", v.SNR[i]);
    }
    s << indent << "LLI[]" << std::endl;
    for (size_t i = 0; i < v.LLI.size(); ++i)
    {
      s << indent << "  LLI[" << i << "]: ";
      Printer<uint8_t>::stream(s, indent + "  ", v.LLI[i]);
    }
    s << indent << "code[]" << std::endl;
    for (size_t i = 0; i < v.code.size(); ++i)
    {
      s << indent << "  code[" << i << "]: ";
      Printer<uint8_t>::stream(s, indent + "  ", v.code[i]);
    }
    s << indent << "L[]" << std::endl;
    for (size_t i = 0; i < v.L.size(); ++i)
    {
      s << indent << "  L[" << i << "]: ";
      Printer<double>::stream(s, indent + "  ", v.L[i]);
    }
    s << indent << "P[]" << std::endl;
    for (size_t i = 0; i < v.P.size(); ++i)
    {
      s << indent << "  P[" << i << "]: ";
      Printer<double>::stream(s, indent + "  ", v.P[i]);
    }
    s << indent << "D[]" << std::endl;
    for (size_t i = 0; i < v.D.size(); ++i)
    {
      s << indent << "  D[" << i << "]: ";
      Printer<float>::stream(s, indent + "  ", v.D[i]);
    }
  }
};


} // namespace message_operations
} // namespace ros

#endif // RTKRCV_MESSAGE_OBSERVATIONDATA_H
