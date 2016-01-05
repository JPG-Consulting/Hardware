import serial
import sys
while True:
    try:
        ser = serial.Serial('COM4', 9600)
    except Exception, e:
        print 'open serial failed.'
        exit(1)
    str = ser.readline()
    str = str.split(':')
    temp = ""
    temp = str[1].strip()
    temp += '\t'
    temp += str[3].strip()
    print temp
    sys.stdout.flush()
    ser.close()

#output: 2-tuple (pressure, temperture)
