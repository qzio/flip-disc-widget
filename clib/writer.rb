#!/usr/bin/env ruby
require 'rubygems'
require 'serialport'




def mk_msg(str)
  s = "\xff\x06\xa2"
  crc = 6+162
  arr = str.split("")
  for t in arr do
    s += [t].pack("B")
  end



  s
end


ser = Serialport.new "/dev/ttyUSB0", 4800
ser.write
