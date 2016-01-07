import serial
import sys
import random
import time
import json
import urllib
import urllib2

apiurldooruser  = "http://192.168.199.187/sensordata/rfiddooruser"
apiurldoorbook  = "http://192.168.199.187/sensordata/rfiddoorbook"
apiurlshelfuser   = "http://192.168.199.187/sensordata/rfidshelfuser"
apiurlshelfbook  = "http://192.168.199.187/sensordata/rfidshelfbook"
data = {}

status = 0

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
    code = "\x02\x02\x00\xA0\x05\x00\x00\x00\xA0\x00\x01"
    n = ser.write(code)
    str = ser.read(15)
    str = str.strip()
    str = str.split()
    ID = ""
    for x in str[0]:
        ID += ("%02x" % ord(x)).upper() + " "
    ID = ID.strip()
    ID = ID.replace(" ", "")
    data['id'] = ID
    data = json.dumps(data)
    print data
    print post(apiurlshelfbook, data)
    ser.close()

#output: 1 string (RFID ID)
