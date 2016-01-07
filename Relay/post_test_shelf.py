import urllib
import urllib2
import json

url = "http://192.168.199.187/debug/relayshelf"
command = {"command" : 0}
command = json.dumps(command)
req = urllib2.Request(url, command)
response = urllib2.urlopen(req)
res = response.read()
