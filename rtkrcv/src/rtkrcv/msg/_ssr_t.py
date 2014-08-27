"""autogenerated by genpy from rtkrcv/ssr_t.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct

import genpy

class ssr_t(genpy.Message):
  _md5sum = "7f15313cb606c32dfc95c29506d2fce0"
  _type = "rtkrcv/ssr_t"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """ # SSR correction type

time t0             # epoch time (GPST)
float64[] udi       # SSR update interval (s)
int32[] iod
int32 iode          # issue of data
int32 ura           # URA indicator
int32 refd          # sat ref datum (0:ITRF,1:regional)
float64[] deph      # delta orbit {radial,along,cross} (m)
float64[] ddeph     # dot delta orbit {radial,along,cross} (m/s)
float64[] dclk      # delta clock {c0,c1,c2} (m,m/s,m/s^2)
float64 hrclk       # high-rate clock corection (m)
float32[] cbias     # code biases (m)
uint8 update        # update flag (0:no update,1:update)
"""
  __slots__ = ['t0','udi','iod','iode','ura','refd','deph','ddeph','dclk','hrclk','cbias','update']
  _slot_types = ['time','float64[]','int32[]','int32','int32','int32','float64[]','float64[]','float64[]','float64','float32[]','uint8']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       t0,udi,iod,iode,ura,refd,deph,ddeph,dclk,hrclk,cbias,update

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(ssr_t, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.t0 is None:
        self.t0 = genpy.Time()
      if self.udi is None:
        self.udi = []
      if self.iod is None:
        self.iod = []
      if self.iode is None:
        self.iode = 0
      if self.ura is None:
        self.ura = 0
      if self.refd is None:
        self.refd = 0
      if self.deph is None:
        self.deph = []
      if self.ddeph is None:
        self.ddeph = []
      if self.dclk is None:
        self.dclk = []
      if self.hrclk is None:
        self.hrclk = 0.
      if self.cbias is None:
        self.cbias = []
      if self.update is None:
        self.update = 0
    else:
      self.t0 = genpy.Time()
      self.udi = []
      self.iod = []
      self.iode = 0
      self.ura = 0
      self.refd = 0
      self.deph = []
      self.ddeph = []
      self.dclk = []
      self.hrclk = 0.
      self.cbias = []
      self.update = 0

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
      buff.write(_struct_2I.pack(_x.t0.secs, _x.t0.nsecs))
      length = len(self.udi)
      buff.write(_struct_I.pack(length))
      pattern = '<%sd'%length
      buff.write(struct.pack(pattern, *self.udi))
      length = len(self.iod)
      buff.write(_struct_I.pack(length))
      pattern = '<%si'%length
      buff.write(struct.pack(pattern, *self.iod))
      _x = self
      buff.write(_struct_3i.pack(_x.iode, _x.ura, _x.refd))
      length = len(self.deph)
      buff.write(_struct_I.pack(length))
      pattern = '<%sd'%length
      buff.write(struct.pack(pattern, *self.deph))
      length = len(self.ddeph)
      buff.write(_struct_I.pack(length))
      pattern = '<%sd'%length
      buff.write(struct.pack(pattern, *self.ddeph))
      length = len(self.dclk)
      buff.write(_struct_I.pack(length))
      pattern = '<%sd'%length
      buff.write(struct.pack(pattern, *self.dclk))
      buff.write(_struct_d.pack(self.hrclk))
      length = len(self.cbias)
      buff.write(_struct_I.pack(length))
      pattern = '<%sf'%length
      buff.write(struct.pack(pattern, *self.cbias))
      buff.write(_struct_B.pack(self.update))
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
      end += 8
      (_x.t0.secs, _x.t0.nsecs,) = _struct_2I.unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%sd'%length
      start = end
      end += struct.calcsize(pattern)
      self.udi = struct.unpack(pattern, str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%si'%length
      start = end
      end += struct.calcsize(pattern)
      self.iod = struct.unpack(pattern, str[start:end])
      _x = self
      start = end
      end += 12
      (_x.iode, _x.ura, _x.refd,) = _struct_3i.unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%sd'%length
      start = end
      end += struct.calcsize(pattern)
      self.deph = struct.unpack(pattern, str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%sd'%length
      start = end
      end += struct.calcsize(pattern)
      self.ddeph = struct.unpack(pattern, str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%sd'%length
      start = end
      end += struct.calcsize(pattern)
      self.dclk = struct.unpack(pattern, str[start:end])
      start = end
      end += 8
      (self.hrclk,) = _struct_d.unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%sf'%length
      start = end
      end += struct.calcsize(pattern)
      self.cbias = struct.unpack(pattern, str[start:end])
      start = end
      end += 1
      (self.update,) = _struct_B.unpack(str[start:end])
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
      buff.write(_struct_2I.pack(_x.t0.secs, _x.t0.nsecs))
      length = len(self.udi)
      buff.write(_struct_I.pack(length))
      pattern = '<%sd'%length
      buff.write(self.udi.tostring())
      length = len(self.iod)
      buff.write(_struct_I.pack(length))
      pattern = '<%si'%length
      buff.write(self.iod.tostring())
      _x = self
      buff.write(_struct_3i.pack(_x.iode, _x.ura, _x.refd))
      length = len(self.deph)
      buff.write(_struct_I.pack(length))
      pattern = '<%sd'%length
      buff.write(self.deph.tostring())
      length = len(self.ddeph)
      buff.write(_struct_I.pack(length))
      pattern = '<%sd'%length
      buff.write(self.ddeph.tostring())
      length = len(self.dclk)
      buff.write(_struct_I.pack(length))
      pattern = '<%sd'%length
      buff.write(self.dclk.tostring())
      buff.write(_struct_d.pack(self.hrclk))
      length = len(self.cbias)
      buff.write(_struct_I.pack(length))
      pattern = '<%sf'%length
      buff.write(self.cbias.tostring())
      buff.write(_struct_B.pack(self.update))
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
      end += 8
      (_x.t0.secs, _x.t0.nsecs,) = _struct_2I.unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%sd'%length
      start = end
      end += struct.calcsize(pattern)
      self.udi = numpy.frombuffer(str[start:end], dtype=numpy.float64, count=length)
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%si'%length
      start = end
      end += struct.calcsize(pattern)
      self.iod = numpy.frombuffer(str[start:end], dtype=numpy.int32, count=length)
      _x = self
      start = end
      end += 12
      (_x.iode, _x.ura, _x.refd,) = _struct_3i.unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%sd'%length
      start = end
      end += struct.calcsize(pattern)
      self.deph = numpy.frombuffer(str[start:end], dtype=numpy.float64, count=length)
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%sd'%length
      start = end
      end += struct.calcsize(pattern)
      self.ddeph = numpy.frombuffer(str[start:end], dtype=numpy.float64, count=length)
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%sd'%length
      start = end
      end += struct.calcsize(pattern)
      self.dclk = numpy.frombuffer(str[start:end], dtype=numpy.float64, count=length)
      start = end
      end += 8
      (self.hrclk,) = _struct_d.unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%sf'%length
      start = end
      end += struct.calcsize(pattern)
      self.cbias = numpy.frombuffer(str[start:end], dtype=numpy.float32, count=length)
      start = end
      end += 1
      (self.update,) = _struct_B.unpack(str[start:end])
      self.t0.canon()
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_3i = struct.Struct("<3i")
_struct_B = struct.Struct("<B")
_struct_2I = struct.Struct("<2I")
_struct_d = struct.Struct("<d")