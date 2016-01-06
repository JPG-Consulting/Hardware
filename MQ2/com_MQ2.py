import serial
import sys
import urllib
import urllib2
import time
import json

apiurl = "http://192.168.199.187/sensordata/mq2"
data = {}

def post(url, data):
    #data = urllib.urlencode(data)
    req = urllib2.Request(url, data)
    response = urllib2.urlopen(req)
    res = response.read()
    return res

while True:
    data = {}
    try:
        ser = serial.Serial('COM4', 9600)
    except Exception, e:
        print 'open serial failed.'
        exit(1)
    str = ser.readline()
    data['gas_sample'] = float(str.split()[1])
    data = json.dumps(data)
    print data
    print post(apiurl, data)

    sys.stdout.flush()
    ser.close()
    time.sleep(1)

#output: 1 string (gas qualitative concentration)
