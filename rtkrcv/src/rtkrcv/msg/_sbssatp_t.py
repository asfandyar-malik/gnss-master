"""autogenerated by genpy from rtkrcv/sbssatp_t.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct

import geometry_msgs.msg
import genpy
import rtkrcv.msg

class sbssatp_t(genpy.Message):
  _md5sum = "4650f2d5bd674ec683ed24347dc0706f"
  _type = "rtkrcv/sbssatp_t"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """# SBAS satellite correction type

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
  __slots__ = ['sat','fcorr','lcorr']
  _slot_types = ['int32','rtkrcv/SbasFastCorrection','rtkrcv/SbasLongTermErrorCorrection']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       sat,fcorr,lcorr

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(sbssatp_t, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.sat is None:
        self.sat = 0
      if self.fcorr is None:
        self.fcorr = rtkrcv.msg.SbasFastCorrection()
      if self.lcorr is None:
        self.lcorr = rtkrcv.msg.SbasLongTermErrorCorrection()
    else:
      self.sat = 0
      self.fcorr = rtkrcv.msg.SbasFastCorrection()
      self.lcorr = rtkrcv.msg.SbasLongTermErrorCorrection()

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
      buff.write(_struct_i2I3di2h2Ii8d.pack(_x.sat, _x.fcorr.t0.secs, _x.fcorr.t0.nsecs, _x.fcorr.prc, _x.fcorr.rrc, _x.fcorr.dt, _x.fcorr.iodf, _x.fcorr.udre, _x.fcorr.ai, _x.lcorr.t0.secs, _x.lcorr.t0.nsecs, _x.lcorr.iode, _x.lcorr.dpos.x, _x.lcorr.dpos.y, _x.lcorr.dpos.z, _x.lcorr.dvel.x, _x.lcorr.dvel.y, _x.lcorr.dvel.z, _x.lcorr.daf0, _x.lcorr.daf1s))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      if self.fcorr is None:
        self.fcorr = rtkrcv.msg.SbasFastCorrection()
      if self.lcorr is None:
        self.lcorr = rtkrcv.msg.SbasLongTermErrorCorrection()
      end = 0
      _x = self
      start = end
      end += 120
      (_x.sat, _x.fcorr.t0.secs, _x.fcorr.t0.nsecs, _x.fcorr.prc, _x.fcorr.rrc, _x.fcorr.dt, _x.fcorr.iodf, _x.fcorr.udre, _x.fcorr.ai, _x.lcorr.t0.secs, _x.lcorr.t0.nsecs, _x.lcorr.iode, _x.lcorr.dpos.x, _x.lcorr.dpos.y, _x.lcorr.dpos.z, _x.lcorr.dvel.x, _x.lcorr.dvel.y, _x.lcorr.dvel.z, _x.lcorr.daf0, _x.lcorr.daf1s,) = _struct_i2I3di2h2Ii8d.unpack(str[start:end])
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
      buff.write(_struct_i2I3di2h2Ii8d.pack(_x.sat, _x.fcorr.t0.secs, _x.fcorr.t0.nsecs, _x.fcorr.prc, _x.fcorr.rrc, _x.fcorr.dt, _x.fcorr.iodf, _x.fcorr.udre, _x.fcorr.ai, _x.lcorr.t0.secs, _x.lcorr.t0.nsecs, _x.lcorr.iode, _x.lcorr.dpos.x, _x.lcorr.dpos.y, _x.lcorr.dpos.z, _x.lcorr.dvel.x, _x.lcorr.dvel.y, _x.lcorr.dvel.z, _x.lcorr.daf0, _x.lcorr.daf1s))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      if self.fcorr is None:
        self.fcorr = rtkrcv.msg.SbasFastCorrection()
      if self.lcorr is None:
        self.lcorr = rtkrcv.msg.SbasLongTermErrorCorrection()
      end = 0
      _x = self
      start = end
      end += 120
      (_x.sat, _x.fcorr.t0.secs, _x.fcorr.t0.nsecs, _x.fcorr.prc, _x.fcorr.rrc, _x.fcorr.dt, _x.fcorr.iodf, _x.fcorr.udre, _x.fcorr.ai, _x.lcorr.t0.secs, _x.lcorr.t0.nsecs, _x.lcorr.iode, _x.lcorr.dpos.x, _x.lcorr.dpos.y, _x.lcorr.dpos.z, _x.lcorr.dvel.x, _x.lcorr.dvel.y, _x.lcorr.dvel.z, _x.lcorr.daf0, _x.lcorr.daf1s,) = _struct_i2I3di2h2Ii8d.unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_i2I3di2h2Ii8d = struct.Struct("<i2I3di2h2Ii8d")
