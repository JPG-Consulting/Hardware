import serial
import sys
while True:
    try:
        ser = serial.Serial('COM4', 9600)
    except Exception, e:
        print 'open serial failed.'
        exit(1)
    str = ser.readline()
    temp = str.strip()
    temp += '\t'
    print temp
    sys.stdout.flush()
    ser.close()

#output: 1 byte char (status)
#1:magnet in position
#0:out of range
