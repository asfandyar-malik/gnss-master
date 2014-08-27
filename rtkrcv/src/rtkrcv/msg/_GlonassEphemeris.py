"""autogenerated by genpy from rtkrcv/GlonassEphemeris.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct

import geometry_msgs.msg
import genpy

class GlonassEphemeris(genpy.Message):
  _md5sum = "1bcea641826dae925fbfe0b21d6b21d3"
  _type = "rtkrcv/GlonassEphemeris"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """# GLONASS broadcast ephemeris type

int32 sat                      # satellite number
int32 iode                     # IODE (0-6 bit of tb field)
int32 frq                      # satellite frequency number
int32 svh                      # satellite health
int32 sva                      # satellite accuracy
int32 age                      # satellite age of operation

time toe                       # epoch of epherides (gpst)
time tof                       # message frame time (gpst)

geometry_msgs/Vector3 pos      # satellite position (ecef) (m)
geometry_msgs/Vector3 vel      # satellite velocity (ecef) (m/s)
geometry_msgs/Vector3 acc      # satellite acceleration (ecef) (m/s^2)
float64 taun                   # SV clock bias (s)
float64 gamn                   # relative freq bias
float64 dtaun                  # delay between L1 and L2 (s)
================================================================================
MSG: geometry_msgs/Vector3
# This represents a vector in free space. 

float64 x
float64 y
float64 z
"""
  __slots__ = ['sat','iode','frq','svh','sva','age','toe','tof','pos','vel','acc','taun','gamn','dtaun']
  _slot_types = ['int32','int32','int32','int32','int32','int32','time','time','geometry_msgs/Vector3','geometry_msgs/Vector3','geometry_msgs/Vector3','float64','float64','float64']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       sat,iode,frq,svh,sva,age,toe,tof,pos,vel,acc,taun,gamn,dtaun

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(GlonassEphemeris, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.sat is None:
        self.sat = 0
      if self.iode is None:
        self.iode = 0
      if self.frq is None:
        self.frq = 0
      if self.svh is None:
        self.svh = 0
      if self.sva is None:
        self.sva = 0
      if self.age is None:
        self.age = 0
      if self.toe is None:
        self.toe = genpy.Time()
      if self.tof is None:
        self.tof = genpy.Time()
      if self.pos is None:
        self.pos = geometry_msgs.msg.Vector3()
      if self.vel is None:
        self.vel = geometry_msgs.msg.Vector3()
      if self.acc is None:
        self.acc = geometry_msgs.msg.Vector3()
      if self.taun is None:
        self.taun = 0.
      if self.gamn is None:
        self.gamn = 0.
      if self.dtaun is None:
        self.dtaun = 0.
    else:
      self.sat = 0
      self.iode = 0
      self.frq = 0
      self.svh = 0
      self.sva = 0
      self.age = 0
      self.toe = genpy.Time()
      self.tof = genpy.Time()
      self.pos = geometry_msgs.msg.Vector3()
      self.vel = geometry_msgs.msg.Vector3()
      self.acc = geometry_msgs.msg.Vector3()
      self.taun = 0.
      self.gamn = 0.
      self.dtaun = 0.

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      _x = self
      buff.write(_struct_6i4I12d.pack(_x.sat, _x.iode, _x.frq, _x.svh, _x.sva, _x.age, _x.toe.secs, _x.toe.nsecs, _x.tof.secs, _x.tof.nsecs, _x.pos.x, _x.pos.y, _x.pos.z, _x.vel.x, _x.vel.y, _x.vel.z, _x.acc.x, _x.acc.y, _x.acc.z, _x.taun, _x.gamn, _x.dtaun))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      if self.toe is None:
        self.toe = genpy.Time()
      if self.tof is None:
        self.tof = genpy.Time()
      if self.pos is None:
        self.pos = geometry_msgs.msg.Vector3()
      if self.vel is None:
        self.vel = geometry_msgs.msg.Vector3()
      if self.acc is None:
        self.acc = geometry_msgs.msg.Vector3()
      end = 0
      _x = self
      start = end
      end += 136
      (_x.sat, _x.iode, _x.frq, _x.svh, _x.sva, _x.age, _x.toe.secs, _x.toe.nsecs, _x.tof.secs, _x.tof.nsecs, _x.pos.x, _x.pos.y, _x.pos.z, _x.vel.x, _x.vel.y, _x.vel.z, _x.acc.x, _x.acc.y, _x.acc.z, _x.taun, _x.gamn, _x.dtaun,) = _struct_6i4I12d.unpack(str[start:end])
      self.toe.canon()
      self.tof.canon()
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      _x = self
      buff.write(_struct_6i4I12d.pack(_x.sat, _x.iode, _x.frq, _x.svh, _x.sva, _x.age, _x.toe.secs, _x.toe.nsecs, _x.tof.secs, _x.tof.nsecs, _x.pos.x, _x.pos.y, _x.pos.z, _x.vel.x, _x.vel.y, _x.vel.z, _x.acc.x, _x.acc.y, _x.acc.z, _x.taun, _x.gamn, _x.dtaun))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      if self.toe is None:
        self.toe = genpy.Time()
      if self.tof is None:
        self.tof = genpy.Time()
      if self.pos is None:
        self.pos = geometry_msgs.msg.Vector3()
      if self.vel is None:
        self.vel = geometry_msgs.msg.Vector3()
      if self.acc is None:
        self.acc = geometry_msgs.msg.Vector3()
      end = 0
      _x = self
      start = end
      end += 136
      (_x.sat, _x.iode, _x.frq, _x.svh, _x.sva, _x.age, _x.toe.secs, _x.toe.nsecs, _x.tof.secs, _x.tof.nsecs, _x.pos.x, _x.pos.y, _x.pos.z, _x.vel.x, _x.vel.y, _x.vel.z, _x.acc.x, _x.acc.y, _x.acc.z, _x.taun, _x.gamn, _x.dtaun,) = _struct_6i4I12d.unpack(str[start:end])
      self.toe.canon()
      self.tof.canon()
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_6i4I12d = struct.Struct("<6i4I12d")