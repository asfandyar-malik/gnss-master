"""autogenerated by genpy from rtkrcv/SatelliteObservation.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct

import rtkrcv.msg
import std_msgs.msg

class SatelliteObservation(genpy.Message):
  _md5sum = "fd47f98000c5ba9f9e14c23b35fabebd"
  _type = "rtkrcv/SatelliteObservation"
  _has_header = True #flag to mark the presence of a Header object
  _full_text = """#Satellite observation
Header header


#Number of the satellite (1-32 for GPS)
uint8 sat


#Signal observations for different frequencies
SignalObservation[] observations
================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.secs: seconds (stamp_secs) since epoch
# * stamp.nsecs: nanoseconds since stamp_secs
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
# 0: no frame
# 1: global frame
string frame_id

================================================================================
MSG: rtkrcv/SignalObservation
#Signal to noise ratio (0.25 dBHz)
uint8 snr
#Loss of lock indicator
uint8 lli
#Code indicator
uint8 sig_code
#Observation data carrier-phase (cycle)
float64 carrier_phase
#Observation data pseudorange (m)
float64 pseudorange
#Observation data doppler frequency (Hz)
float32 doppler

uint8 SIG_CODE_NONE = 0     # obs code: none or unknown
uint8 SIG_CODE_L1C  = 1     # obs code: L1C/A,E1C  (GPS,GLO,GAL,QZS,SBS)
uint8 SIG_CODE_L1P  = 2     # obs code: L1P        (GPS,GLO)
uint8 SIG_CODE_L1W  = 3     # obs code: L1 Z-track (GPS)
uint8 SIG_CODE_L1Y  = 4     # obs code: L1Y        (GPS)
uint8 SIG_CODE_L1M  = 5     # obs code: L1M        (GPS)
uint8 SIG_CODE_L1N  = 6     # obs code: L1codeless (GPS)
uint8 SIG_CODE_L1S  = 7     # obs code: L1C(D)     (GPS,QZS)
uint8 SIG_CODE_L1L  = 8     # obs code: L1C(P)     (GPS,QZS)
uint8 SIG_CODE_L1E  = 9     # obs code: L1-SAIF    (QZS)
uint8 SIG_CODE_L1A  = 10    # obs code: E1A        (GAL)
uint8 SIG_CODE_L1B  = 11    # obs code: E1B        (GAL)
uint8 SIG_CODE_L1X  = 12    # obs code: E1B+C,L1C(D+P) (GAL,QZS)
uint8 SIG_CODE_L1Z  = 13    # obs code: E1A+B+C    (GAL)
uint8 SIG_CODE_L2C  = 14    # obs code: L2C/A      (GPS,GLO)
uint8 SIG_CODE_L2D  = 15    # obs code: L2 L1C/A-(P2-P1) (GPS)
uint8 SIG_CODE_L2S  = 16    # obs code: L2C(M)     (GPS,QZS)
uint8 SIG_CODE_L2L  = 17    # obs code: L2C(L)     (GPS,QZS)
uint8 SIG_CODE_L2X  = 18    # obs code: L2C(M+L)   (GPS,QZS)
uint8 SIG_CODE_L2P  = 19    # obs code: L2P        (GPS,GLO)
uint8 SIG_CODE_L2W  = 20    # obs code: L2 Z-track (GPS)
uint8 SIG_CODE_L2Y  = 21    # obs code: L2Y        (GPS)
uint8 SIG_CODE_L2M  = 22    # obs code: L2M        (GPS)
uint8 SIG_CODE_L2N  = 23    # obs code: L2codeless (GPS)
uint8 SIG_CODE_L5I  = 24    # obs code: L5/E5aI    (GPS,GAL,QZS,SBS)
uint8 SIG_CODE_L5Q  = 25    # obs code: L5/E5aQ    (GPS,GAL,QZS,SBS)
uint8 SIG_CODE_L5X  = 26    # obs code: L5/E5aI+Q  (GPS,GAL,QZS,SBS)
uint8 SIG_CODE_L7I  = 27    # obs code: E5bI       (GAL)
uint8 SIG_CODE_L7Q  = 28    # obs code: E5bQ       (GAL)
uint8 SIG_CODE_L7X  = 29    # obs code: E5bI+Q     (GAL)
uint8 SIG_CODE_L6A  = 30    # obs code: E6A        (GAL)
uint8 SIG_CODE_L6B  = 31    # obs code: E6B        (GAL)
uint8 SIG_CODE_L6C  = 32    # obs code: E6C        (GAL)
uint8 SIG_CODE_L6X  = 33    # obs code: E6B+C      (GAL)
uint8 SIG_CODE_L6Z  = 34    # obs code: E6A+B+C    (GAL)
uint8 SIG_CODE_L6S  = 35    # obs code: LEX-S      (QZS)
uint8 SIG_CODE_L6L  = 36    # obs code: LEX-L      (QZS)
uint8 SIG_CODE_L8I  = 37    # obs code: E5(a+b)I   (GAL)
uint8 SIG_CODE_L8Q  = 38    # obs code: E5(a+b)Q   (GAL)
uint8 SIG_CODE_L8X  = 39    # obs code: E5(a+b)I+Q (GAL)
uint8 MAXSIGCODE   = 39    # max number of obs code
"""
  __slots__ = ['header','sat','observations']
  _slot_types = ['std_msgs/Header','uint8','rtkrcv/SignalObservation[]']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       header,sat,observations

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(SatelliteObservation, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.header is None:
        self.header = std_msgs.msg.Header()
      if self.sat is None:
        self.sat = 0
      if self.observations is None:
        self.observations = []
    else:
      self.header = std_msgs.msg.Header()
      self.sat = 0
      self.observations = []

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
      buff.write(_struct_3I.pack(_x.header.seq, _x.header.stamp.secs, _x.header.stamp.nsecs))
      _x = self.header.frame_id
      length = len(_x)
      if python3 or type(_x) == unicode:
        _x = _x.encode('utf-8')
        length = len(_x)
      buff.write(struct.pack('<I%ss'%length, length, _x))
      buff.write(_struct_B.pack(self.sat))
      length = len(self.observations)
      buff.write(_struct_I.pack(length))
      for val1 in self.observations:
        _x = val1
        buff.write(_struct_3B2df.pack(_x.snr, _x.lli, _x.sig_code, _x.carrier_phase, _x.pseudorange, _x.doppler))
    except struct.error as se: self._check_types(se)
    except TypeError as te: self._check_types(te)

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      if self.header is None:
        self.header = std_msgs.msg.Header()
      if self.observations is None:
        self.observations = None
      end = 0
      _x = self
      start = end
      end += 12
      (_x.header.seq, _x.header.stamp.secs, _x.header.stamp.nsecs,) = _struct_3I.unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      start = end
      end += length
      if python3:
        self.header.frame_id = str[start:end].decode('utf-8')
      else:
        self.header.frame_id = str[start:end]
      start = end
      end += 1
      (self.sat,) = _struct_B.unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.observations = []
      for i in range(0, length):
        val1 = rtkrcv.msg.SignalObservation()
        _x = val1
        start = end
        end += 23
        (_x.snr, _x.lli, _x.sig_code, _x.carrier_phase, _x.pseudorange, _x.doppler,) = _struct_3B2df.unpack(str[start:end])
        self.observations.append(val1)
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
      buff.write(_struct_3I.pack(_x.header.seq, _x.header.stamp.secs, _x.header.stamp.nsecs))
      _x = self.header.frame_id
      length = len(_x)
      if python3 or type(_x) == unicode:
        _x = _x.encode('utf-8')
        length = len(_x)
      buff.write(struct.pack('<I%ss'%length, length, _x))
      buff.write(_struct_B.pack(self.sat))
      length = len(self.observations)
      buff.write(_struct_I.pack(length))
      for val1 in self.observations:
        _x = val1
        buff.write(_struct_3B2df.pack(_x.snr, _x.lli, _x.sig_code, _x.carrier_phase, _x.pseudorange, _x.doppler))
    except struct.error as se: self._check_types(se)
    except TypeError as te: self._check_types(te)

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      if self.header is None:
        self.header = std_msgs.msg.Header()
      if self.observations is None:
        self.observations = None
      end = 0
      _x = self
      start = end
      end += 12
      (_x.header.seq, _x.header.stamp.secs, _x.header.stamp.nsecs,) = _struct_3I.unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      start = end
      end += length
      if python3:
        self.header.frame_id = str[start:end].decode('utf-8')
      else:
        self.header.frame_id = str[start:end]
      start = end
      end += 1
      (self.sat,) = _struct_B.unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      self.observations = []
      for i in range(0, length):
        val1 = rtkrcv.msg.SignalObservation()
        _x = val1
        start = end
        end += 23
        (_x.snr, _x.lli, _x.sig_code, _x.carrier_phase, _x.pseudorange, _x.doppler,) = _struct_3B2df.unpack(str[start:end])
        self.observations.append(val1)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_3I = struct.Struct("<3I")
_struct_B = struct.Struct("<B")
_struct_3B2df = struct.Struct("<3B2df")
