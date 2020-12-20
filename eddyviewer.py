#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
#  eddyviewer.py
#  
#  Copyright Colin Luoma 2020  <colin.luoma@gmail.com>
#  

from picamera import PiCamera
from time import sleep
import time
import requests


def main():
	
	camera = PiCamera()
	camera.resolution = (1280, 720)
	
	while True:
	
		start = time.time()
	
		camera.capture('/home/pi/Documents/eddyviewer/image.jpg')
	
		ev_url = 'http://192.168.0.122:9090/api/picture/new'
		image = {'picture': open('/home/pi/Documents/eddyviewer/image.jpg', 'rb')}
		text = {'createddate': str(int(time.time()))}
		
		try:
			r = requests.post(ev_url, files=image, data=text)
		except requests.exceptions.HTTPError as errh:
			print ("Http Error:",errh)
		except requests.exceptions.ConnectionError as errc:
			print ("Error Connecting:",errc)
		except requests.exceptions.Timeout as errt:
			print ("Timeout Error:",errt)
		except requests.exceptions.RequestException as err:
			print ("General error",err)
	
		print(time.time() - start)
		sleep(5 - (time.time() - start))
	
	return 0

if __name__ == '__main__':
	main()

