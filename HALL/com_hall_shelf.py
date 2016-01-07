import serial
import sys
import time
import json
import urllib
import urllib2

apiurldoor = "http://192.168.199.187/sensordata/halldoor"
apiurlshelf = "http://192.168.199.187/sensordata/hallshelf"
data = {}

status = 0

def post(url, data):
    #data = urllib.urlencode(data)
    req = urllib2.Request(url, data)
    response = urllib2.urlopen(req)
    res = response.read()
    return res

while True:
    str = ""
    data = {}
    try:
        ser = serial.Serial('COM3', 9600)
    except Exception, e:
        print 'open serial failed.'
        exit(1)
    str = ser.readline()
    temp = str.strip()
    if temp != status:
        status = temp
        data['hallstatus'] = int(temp)
        data = json.dumps(data)
        print data
        print post(apiurlshelf, data)
    sys.stdout.flush()
    ser.close()

#output: 1 byte char (status)
#1:magnet in position
#0:out of range
