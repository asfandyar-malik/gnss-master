"""autogenerated by genpy from rtkrcv/sbssat_t.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct

import geometry_msgs.msg
import genpy
import rtkrcv.msg

class sbssat_t(genpy.Message):
  _md5sum = "1c04284e2f92ed53fc55e7d63dfd1dc7"
  _type = "rtkrcv/sbssat_t"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """# SBAS satellite corrections type

int32 iodp               # IODP (issue of date mask)
int32 nsat               # number of satellites
int32 tlat               # system latency (s)
sbssatp_t[] sat          # satellite correction
================================================================================
MSG: rtkrcv/sbssatp_t
# SBAS satellite correction type

int32 sat                                   # satellite number
SbasFastCorrection fcorr                    # fast correction
SbasLongTermErrorCorrection lcorr           # long term correction
================================================================================
MSG: rtkrcv/SbasFastCorrection
# SBAS fast correction type

time t0             # time of applicability (TOF)
float64 prc         # pseudorange correction (PRC) (m)
float64 rrc         # range-rate correction (RRC) (m/s)
float64 dt          # range-rate correction delta-time (s)
int32 iodf          # IODF (issue of date fast corr)
int16 udre          # UDRE+1
int16 ai            # degradation factor indicator
================================================================================
MSG: rtkrcv/SbasLongTermErrorCorrection
# SBAS long term satellite error correction type

time t0                        # correction time
int32 iode                     # IODE (issue of date ephemeris)
geometry_msgs/Vector3 dpos     # delta position (m) (ecef)
geometry_msgs/Vector3 dvel     # delta velocity (m/s) (ecef)
float64 daf0                   # delta clock-offset(s,s/s)
float64 daf1s                  # delta drift (s,s/s)

================================================================================
MSG: geometry_msgs/Vector3
# This represents a vector in free space. 

float64 x
float64 y
float64 z
"""
  __slots__ = ['iodp','nsat','tlat','sat']
  _slot_types = ['int32','int32','int32','rtkrcv/sbssatp_t[]']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       iodp,nsat,tlat,sat

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(sbssat_t, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.iodp is None:
        self.iodp = 0
      if self.nsat is None:
        self.nsat = 0
      if self.tlat is None:
        self.tlat = 0
      if self.sat is None:
        self.sat = []
    else:
      self.iodp = 0
      self.nsat = 0
      self.tlat = 0
      self.sat = []

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
      buff.write(_struct_3i.pack(_x.iodp, _x.nsat, _x.tlat))
      length = len(self.sat)
      buff.write(_struct_I.pack(length))
      for val1 in self.sat:
        buff.write(_struct_i.pack(val1.sat))
        _v1 = val1.fcorr
        _v2 = _v1.t0
        _x = _v2
        buff.write(_struct_2I.pack(_x.secs, _x.nsecs))
        _x = _v1
        buff.write(_struct_3di2h.pack(_x.prc, _x.rrc, _x.dt, _x.iodf, _x.udre, _x.ai))
        _v3 = val1.lcorr
        _v4 = _v3.t0
        _x = _v4
        buff.write(_struct_2I.pack(_x.secs, _x.nsecs))
        buff.write(_struct_i.pack(_v3.iode))
        _v5 = _v3.dpos
        _x = _v5
        buff.write(_struct_3d.pack(_x.x, _x.y, _x.z))
        _v6 = _v3.dvel
        _x = _v6
        buff.write(_struct_3d.pack(_x.x, _x.y, _x.z))
        _x = _v3
        buff.write(_struct_2d.pack(_x.daf0, _x.daf1s))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      if self.sat is None:
        self.sat = None
      end = 0
      _x = self
      start = end
      end += 12
      (_x.iodp, _x.nsat, _x.tlat,) = _struct_3i.unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.sat = []
      for i in range(0, length):
        val1 = rtkrcv.msg.sbssatp_t()
        start = end
        end += 4
        (val1.sat,) = _struct_i.unpack(str[start:end])
        _v7 = val1.fcorr
        _v8 = _v7.t0
        _x = _v8
        start = end
        end += 8
        (_x.secs, _x.nsecs,) = _struct_2I.unpack(str[start:end])
        _x = _v7
        start = end
        end += 32
        (_x.prc, _x.rrc, _x.dt, _x.iodf, _x.udre, _x.ai,) = _struct_3di2h.unpack(str[start:end])
        _v9 = val1.lcorr
        _v10 = _v9.t0
        _x = _v10
        start = end
        end += 8
        (_x.secs, _x.nsecs,) = _struct_2I.unpack(str[start:end])
        start = end
        end += 4
        (_v9.iode,) = _struct_i.unpack(str[start:end])
        _v11 = _v9.dpos
        _x = _v11
        start = end
        end += 24
        (_x.x, _x.y, _x.z,) = _struct_3d.unpack(str[start:end])
        _v12 = _v9.dvel
        _x = _v12
        start = end
        end += 24
        (_x.x, _x.y, _x.z,) = _struct_3d.unpack(str[start:end])
        _x = _v9
        start = end
        end += 16
        (_x.daf0, _x.daf1s,) = _struct_2d.unpack(str[start:end])
        self.sat.append(val1)
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
      buff.write(_struct_3i.pack(_x.iodp, _x.nsat, _x.tlat))
      length = len(self.sat)
      buff.write(_struct_I.pack(length))
      for val1 in self.sat:
        buff.write(_struct_i.pack(val1.sat))
        _v13 = val1.fcorr
        _v14 = _v13.t0
        _x = _v14
        buff.write(_struct_2I.pack(_x.secs, _x.nsecs))
        _x = _v13
        buff.write(_struct_3di2h.pack(_x.prc, _x.rrc, _x.dt, _x.iodf, _x.udre, _x.ai))
        _v15 = val1.lcorr
        _v16 = _v15.t0
        _x = _v16
        buff.write(_struct_2I.pack(_x.secs, _x.nsecs))
        buff.write(_struct_i.pack(_v15.iode))
        _v17 = _v15.dpos
        _x = _v17
        buff.write(_struct_3d.pack(_x.x, _x.y, _x.z))
        _v18 = _v15.dvel
        _x = _v18
        buff.write(_struct_3d.pack(_x.x, _x.y, _x.z))
        _x = _v15
        buff.write(_struct_2d.pack(_x.daf0, _x.daf1s))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      if self.sat is None:
        self.sat = None
      end = 0
      _x = self
      start = end
      end += 12
      (_x.iodp, _x.nsat, _x.tlat,) = _struct_3i.unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.sat = []
      for i in range(0, length):
        val1 = rtkrcv.msg.sbssatp_t()
        start = end
        end += 4
        (val1.sat,) = _struct_i.unpack(str[start:end])
        _v19 = val1.fcorr
        _v20 = _v19.t0
        _x = _v20
        start = end
        end += 8
        (_x.secs, _x.nsecs,) = _struct_2I.unpack(str[start:end])
        _x = _v19
        start = end
        end += 32
        (_x.prc, _x.rrc, _x.dt, _x.iodf, _x.udre, _x.ai,) = _struct_3di2h.unpack(str[start:end])
        _v21 = val1.lcorr
        _v22 = _v21.t0
        _x = _v22
        start = end
        end += 8
        (_x.secs, _x.nsecs,) = _struct_2I.unpack(str[start:end])
        start = end
        end += 4
        (_v21.iode,) = _struct_i.unpack(str[start:end])
        _v23 = _v21.dpos
        _x = _v23
        start = end
        end += 24
        (_x.x, _x.y, _x.z,) = _struct_3d.unpack(str[start:end])
        _v24 = _v21.dvel
        _x = _v24
        start = end
        end += 24
        (_x.x, _x.y, _x.z,) = _struct_3d.unpack(str[start:end])
        _x = _v21
        start = end
        end += 16
        (_x.daf0, _x.daf1s,) = _struct_2d.unpack(str[start:end])
        self.sat.append(val1)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_i = struct.Struct("<i")
_struct_2d = struct.Struct("<2d")
_struct_3i = struct.Struct("<3i")
_struct_3di2h = struct.Struct("<3di2h")
_struct_2I = struct.Struct("<2I")
_struct_3d = struct.Struct("<3d")
