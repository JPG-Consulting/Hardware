import serial
import sys
import random
import time
import json
import urllib
import urllib2

apiurldoor = "http://192.168.199.187/command/relaydoor"
apiurlshelf = "http://192.168.199.187/command/relayshelf"
data = {}

status = 0

def get(url):
    #data = urllib.urlencode(data)
    req = urllib2.Request(url)
    response = urllib2.urlopen(req)
    res = response.read()
    return res

while True:
    try:
        ser = serial.Serial('COM4', 9600)
    except Exception, e:
        print 'open serial failed.'
        exit(1)
    command = get(apiurlshelf)
    #ser.write("%d" % random.randint(0,1))
    if command is not None:
        command = command.split(':')
        ser.write(command[1][0])
        sys.stdout.flush()
    ser.close()

#input: random number of {0, 1}
#0:break
#1:access
