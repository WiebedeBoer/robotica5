import numpy as np
import cv2
import math
import random

def ShowCamFeed():
	cap = cv2.VideoCapture(0)
	while(True):
		_, frame = cap.read()

		frame = cv2.GaussianBlur(frame,(5,5),0)
		frame = cv2.bilateralFilter(frame,9,75,75)
		
		cv2.imshow('frame', frame)

		# HSV
		#lower_red = np.array([24,30,94])
		#upper_red = np.array([26,64,81])

		# RGB
		# lower_red = np.array([227, 150,96])
		# upper_red = np.array([240,196,169])

		#define range of color blue
		lower_blue = np.array([110,50,50])
		upper_blue = np.array([130,255,255])

		lower_white = np.array([65,62,68])
		upper_white = np.array([85,95,75])

		lower_red = np.array([0,0,0])
		upper_red = np.array([30,150,150])
		
		#maskRed = cv2.inRange(frame, lower_red, upper_red)
		maskRed = cv2.inRange(frame, lower_white, upper_white)

		resRed = cv2.bitwise_and(frame,frame, mask=maskRed)
		cv2.imshow('maskresult',resRed)

		res = cv2.cvtColor(resRed, cv2.COLOR_BGR2GRAY)
		cv2.imshow('mask1',res)
		_, res = cv2.threshold(res,100,255,cv2.THRESH_BINARY_INV)
		cv2.imshow('mask',res)


		contours,hierarchy = cv2.findContours(res,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE) 

		for cnt in contours:
			area = cv2.contourArea(cnt)
			perimeter = cv2.arcLength(cnt,True)
			if(perimeter > 0):
				vormfactor = 4 * math.pi * area / perimeter ** 2
				if area > 1500:
					if vormfactor > 0.6:
						x,y,w,h = cv2.boundingRect(cnt)
						frame = cv2.rectangle(frame,(x,y),(x+w,y+h),(0,255,0),2)
						frame = cv2.drawContours(frame, [cnt], -1, (255,0,255), 3)

		cv2.imshow('result', frame)

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break
	cap.release()
	cv2.destroyAllWindows()

ShowCamFeed()
