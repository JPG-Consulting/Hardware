import serial
import sys
while True:
    try:
        ser = serial.Serial('COM4', 9600)
    except Exception, e:
        print 'open serial failed.'
        exit(1)
    str = ser.readline()
    temp = str.split()[1]
    temp += '\t'
    str = ser.readline()
    temp += str.split()[1]
    print temp
    sys.stdout.flush()
    ser.close()

#output: 2-tuple (temperture, humidity)
