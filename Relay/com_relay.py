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
    #str_control = raw_input('enter control code:')
    ser.write("%d" % random.randint(0,1))
    sys.stdout.flush()
    ser.close()
    time.sleep(0.2)

#input: random number of {0, 1}
#0:break
#1:access
