#!/usr/bin/python

#
# Dump timestamps & formatted 12 byte ANT messages from a GC antlog.raw file;
#
# data[0]    sync byte - always 0xA4
# data[1]    message length
# data[2]    message id - 0x4E is broadcast data
# data[3-11] message bytes
#             - for broadcast data, data[3] is the channel number
#

import sys
import os
import struct

file = open(sys.argv[1], 'rb')
for line in range(0, os.path.getsize(sys.argv[1]), 21):

  data = file.read(1)
  RS = str(data[0])

  data = file.read(8)
  timestamp = struct.unpack('Q', data)[0]

  data = file.read(12)
  print (str(timestamp) + ' - ' \
      + RS  + ': ' \
      + '0x' + '{:02X}'.format(data[0]).upper() + ', ' \
      + '0x' + '{:02X}'.format(data[1]).upper() + ', ' \
      + '0x' + '{:02X}'.format(data[2]).upper() + ', ' \
      + '0x' + '{:02X}'.format(data[3]).upper() + ', ' \
      + '0x' + '{:02X}'.format(data[4]).upper() + ', ' \
      + '0x' + '{:02X}'.format(data[5]).upper() + ', ' \
      + '0x' + '{:02X}'.format(data[6]).upper() + ', ' \
      + '0x' + '{:02X}'.format(data[7]).upper() + ', ' \
      + '0x' + '{:02X}'.format(data[8]).upper() + ', ' \
      + '0x' + '{:02X}'.format(data[9]).upper() + ', ' \
      + '0x' + '{:02X}'.format(data[10]).upper() + ', ' \
      + '0x' + '{:02X}'.format(data[11]).upper())

file.close()

