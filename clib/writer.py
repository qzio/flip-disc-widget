#!/usr/bin/env python
import sys
import serial
import struct
from optparse import OptionParser


parser = OptionParser(usage="hurra")
parser.add_option("-p",default="hello world")
options,args = parser.parse_args()

# using default sys.argv
unpacked_str = sys.argv[1]

def mk_msg(txt):
    s = struct.pack("BBB",255,6,162)
    crc = 6+162
    for t in txt:
        s += t
        #s += struct.pack("B",ord(t))
        crc += ord(t)

    #s += struct.pack("B",crc % 256)
    s += chr(crc % 256)
    s += "\xff"
    return s

try:
    ser = serial.Serial("/dev/ttyUSB0", 4800)
    print "serial device initiated"
    print ser
except Exception, e:
    ser = None

#write_string = mk_msg(options.p)
write_string = mk_msg(unpacked_str)

if ser and ser.isOpen():
    print "will write %r" % write_string
    ser.write(write_string)
else:
    print "if the serial port would have been open..."
    print write_string
