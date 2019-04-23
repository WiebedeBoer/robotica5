Python 2.7.13 (v2.7.13:a06454b1afa1, Dec 17 2016, 20:53:40) [MSC v.1500 64 bit (AMD64)] on win32
Type "copyright", "credits" or "license()" for more information.
>>> 
import numpy as np
import cv2
import math

def bluevid():

	cap = cv2.VideoCapture(0)
	while(True):
		ret, frame = cap.read()
		#hsv
		hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
		#define range of color blue
		lower_blue = np.array([110,50,50])
		upper_blue = np.array([130,255,255])
		cv2.imshow('frame',hsv)
                # Bitwise-AND mask and original image
		res = cv2.bitwise_and(frame,frame, mask= mask)
		# Threshold the HSV image to get only blue colors
		mask = cv2.inRange(hsv, lower_blue, upper_blue)
		if cv2.waitKey(1) & 0xFF -- ord('q'):
			break
			cap.release()
		#print k
		k = cv2.waitKey(0)
		print k
		#destroy
		cv2.destroyAllWindows()
