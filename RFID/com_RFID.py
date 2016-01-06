import serial
import sys
import random
import time

while True:
    try:
        ser = serial.Serial('COM4', 9600)
    except Exception, e:
        print 'open serial failed.'
        exit(1)
    code = "\x02\x02\x00\xA0\x05\x00\x00\x00\xA0\x00\x01"
    n = ser.write(code)
    str = ser.read(15)
    str = str.strip()
    str = str.split()
    ID = ""
    for x in str[0]:
        ID += ("%02x" % ord(x)).upper() + " "
    print ID
    ser.close()
    #time.sleep(3)

#output: 1 string (RFID ID)
