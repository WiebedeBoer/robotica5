import numpy as np
import cv2
import math
import random

def ShowCamFeed():
	cap = cv2.VideoCapture(0)
	while(True):
		_, frame = cap.read()

		cv2.imshow('default',frame)
		frame = cv2.bilateralFilter(frame,9,100,100)
		gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)

		# HSV
		#lower_red = np.array([24,30,94])
		#upper_red = np.array([26,64,81])

		lower = np.array([0,50,0])
		upper = np.array([90,180,255])
		
		mask = cv2.inRange(frame, lower, upper)
		cv2.imshow('mask',mask)

		res = cv2.bitwise_and(frame,frame, mask=mask)
		res = cv2.cvtColor(res, cv2.COLOR_RGB2GRAY)
		res = cv2.adaptiveThreshold(res,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,11,2)
		cv2.imshow('maskresult',res)

		contours,hierarchy = cv2.findContours(res,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE) 

		for cnt in contours:
			area = cv2.contourArea(cnt)
			perimeter = cv2.arcLength(cnt,True)
			if(perimeter > 0):
				vormfactor = 4 * math.pi * area / perimeter ** 2
				if area > 500:
					if vormfactor > 0.7:
						x,y,w,h = cv2.boundingRect(cnt)
						frame = cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,255),2)

		cv2.imshow('result', frame)

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break
	cap.release()
	cv2.destroyAllWindows()

ShowCamFeed()