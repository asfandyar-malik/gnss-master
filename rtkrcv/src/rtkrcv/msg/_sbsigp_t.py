"""autogenerated by genpy from rtkrcv/sbsigp_t.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct

import genpy

class sbsigp_t(genpy.Message):
  _md5sum = "f4e8be9b5bc2e0f8af3ad2e777a018eb"
  _type = "rtkrcv/sbsigp_t"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """# SBAS ionospheric correction type

time t0               # correction time
int16 lat             # latitude (deg)
int16 lon             # longitude (deg)
int16 give            # GIVI+1
float32 delay         # vertical delay estimate (m)
"""
  __slots__ = ['t0','lat','lon','give','delay']
  _slot_types = ['time','int16','int16','int16','float32']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       t0,lat,lon,give,delay

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(sbsigp_t, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.t0 is None:
        self.t0 = genpy.Time()
      if self.lat is None:
        self.lat = 0
      if self.lon is None:
        self.lon = 0
      if self.give is None:
        self.give = 0
      if self.delay is None:
        self.delay = 0.
    else:
      self.t0 = genpy.Time()
      self.lat = 0
      self.lon = 0
      self.give = 0
      self.delay = 0.

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
      buff.write(_struct_2I3hf.pack(_x.t0.secs, _x.t0.nsecs, _x.lat, _x.lon, _x.give, _x.delay))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      if self.t0 is None:
        self.t0 = genpy.Time()
      end = 0
      _x = self
      start = end
      end += 18
      (_x.t0.secs, _x.t0.nsecs, _x.lat, _x.lon, _x.give, _x.delay,) = _struct_2I3hf.unpack(str[start:end])
      self.t0.canon()
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
      buff.write(_struct_2I3hf.pack(_x.t0.secs, _x.t0.nsecs, _x.lat, _x.lon, _x.give, _x.delay))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      if self.t0 is None:
        self.t0 = genpy.Time()
      end = 0
      _x = self
      start = end
      end += 18
      (_x.t0.secs, _x.t0.nsecs, _x.lat, _x.lon, _x.give, _x.delay,) = _struct_2I3hf.unpack(str[start:end])
      self.t0.canon()
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_2I3hf = struct.Struct("<2I3hf")
