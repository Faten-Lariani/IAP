# -*- coding: utf-8 -*-
"""
Created on Thu Jan  2 22:54:39 2020

@author: 21655
"""
            
import sys, getopt
import serial
import time
import sys, os
import os.path
from os import path

try:
    from progressbar import *
    usepbar = 1
except:
    usepbar = 0

QUIET = 20

def mdebug(level, message):
        if(QUIET >= level):
            print >> sys.stderr , message


class Interface:
    
    
    
    def open(self, aport='COM3', abaudrate=115200) :
        self.sp = serial.Serial(
            port=aport,
            baudrate=abaudrate,     # baudrates
            bytesize=8,             # number of databits
            parity=serial.PARITY_NONE,
            stopbits=1,
            xonxoff=0,              # don't enable software flow control
            rtscts=0,               # don't enable RTS/CTS flow control
            timeout=5               # set a timeout value, None for waiting forever
        )
    def releaseChip(self):
        self.sp.setRTS(1)
        self.reset()       
        
    def reset(self):
        self.sp.setDTR(0)
        time.sleep(0.1)
        self.sp.setDTR(1)
        time.sleep(0.5)

    def initChip(self):
        # Set boot
        self.sp.setRTS(0)
        self.reset()

#        self.sp.write("\x7F")       # Syncro
#        return self._wait_for_ask("Syncro")
    
    
    
    def cmdWriteMemory(self, data):
        assert(len(data) <= 256)
#        if self.cmdGeneric(0x31):
        mdebug(3, "*** Write memory command")
#            self.sp.write(self._encode_addr(addr))
#            self._wait_for_ask("0x31 address failed")
            #map(lambda c: hex(ord(c)), data)
        lng = (len(data)-1) & 0xFF
        mdebug(3, "    %s bytes to write" % [lng+1]);
#        self.sp.write(chr(lng)) # len really
        crc = 0xFF
        for c in data:
            crc = crc ^ c
            self.sp.write(chr(c))
#        self.sp.write(chr(crc))
#        self._wait_for_ask("0x31 programming failed")
        mdebug(3, "    Write memory done")
        
    def writeMemory(self, addr, data):
        lng = len(data)
        if usepbar:
            widgets = ['Writing: ', Percentage(),' ', ETA(), ' ', Bar()]
            pbar = ProgressBar(widgets=widgets, maxval=lng, term_width=79).start()
        
        offs = 0
        while lng > 256:
            if usepbar:
                pbar.update(pbar.maxval-lng)
            else:
                mdebug(2, "Write %(len)d bytes at 0x%(addr)X" % {'addr': addr, 'len': 256})
            self.cmdWriteMemory(data[offs:offs+256])
#            sys.exit(0)
            offs = offs + 256
            addr = addr + 256
            lng = lng - 256
        if usepbar:
            pbar.update(pbar.maxval-lng)
            pbar.finish()
        else:
            mdebug(2, "Write %(len)d bytes at 0x%(addr)X" % {'addr': addr, 'len': 256})
        self.cmdWriteMemory(data[offs:offs+lng] + ([0xFF] * (256-lng)) )
    
    def sendData(self, addr, flag, data_to_send):
        if flag == "-f":
            # os.path.abspath(data_to_send)
            # file_name="C:\\Users\\21655\\Desktop\\test_boot.bin"
            file_size = os.path.getsize(data_to_send)
            print "******************", file_size
            # self.sp.write(str(file_size))
            data = map(lambda c: ord(c), file(data_to_send, 'rb').read())
            self.writeMemory(0, data)
        else:
            print "DATA ", data_to_send
            self.sp.write(data_to_send)
        
if __name__ == "__main__":
    
    port="COM3"
    baudRate=115200
    interface = Interface()
    interface.open(port, baudRate)
    
    number_arg = len(sys.argv)
    is_signal = 0
    
    if number_arg != 3:
        print "***** Usage", sys.argv[0], " -send/-s 'signal[0/1]' | -file/-f filePath"
        sys.exit(0)
    
    if sys.argv[1] == "-send" or sys.argv[1] == "-s":
        if sys.argv[2] != "0" and sys.argv[2] != "1":
            print "***** Usage", sys.argv[0], " -send/-s 'signal[0/1]' | -file/-f filePath"
            sys.exit(0);
            
        else:
            is_signal = 1
            interface.sendData(0, "-s", sys.argv[2])
            print "***** ", sys.argv[2], "is_signal!"
       
    elif sys.argv[1] == "-file" or sys.argv[1] == "-f":
        if path.exists(sys.argv[2]):
            if path.isfile(sys.argv[2]):
                print "***** ", sys.argv[2], "is a file!"
                interface.sendData(0, "-f", sys.argv[2])
            else:
                print "***** ", sys.argv[2], "is not a file!"
                sys.exit(0)
        else:
             print "***** ", sys.argv[2], "doesn't exist!"
             sys.exit(0)
    else:
         print "***** Usage", sys.argv[0], " -send/-s 'signal[0/1]' | -file/-f filePath"
         sys.exit(0)
    
    

#    os.path.abspath(C:\\Users\\21655\\Desktop\\test_boot.bin")
#    file_name="C:\\Users\\21655\\Desktop\\test_boot.bin"
#    data = map(lambda c: ord(c), file(file_name, 'rb').read())
#    
#
    
    
#    try:
#        try:
#            interface.initChip()
#        except:
#            print "Can't init. Ensure that BOOT0 is enabled and reset device"
#    finally:
#        interface.releaseChip()


            
#ser = serial.Serial("COM3", 115200)
#
#ser.write("Hello world")
#
#x = ser.readline()
#
#print(x)