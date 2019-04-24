import numpy as np
import cv2
import math
import random

def nothing():
	pass

def ShowCamFeed():
	cap = cv2.VideoCapture(0)

	cv2.namedWindow('settings', 2)

	cv2.createTrackbar('H_MIN','settings',0,255,nothing)
	cv2.createTrackbar('S_MIN','settings',0,255,nothing)
	cv2.createTrackbar('V_MIN','settings',0,255,nothing)

	cv2.createTrackbar('H_MAX','settings',0,255,nothing)
	cv2.createTrackbar('S_MAX','settings',0,255,nothing)
	cv2.createTrackbar('V_MAX','settings',0,255,nothing)

	while(True):
		_, frame = cap.read()

		cv2.imshow('default',frame)
		frame = cv2.bilateralFilter(frame,9,100,100)
		gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)

		h_min = cv2.getTrackbarPos('H_MIN','settings')
		s_min = cv2.getTrackbarPos('S_MIN','settings')
		v_min = cv2.getTrackbarPos('V_MIN','settings')

		h_max = cv2.getTrackbarPos('H_MAX','settings')
		s_max = cv2.getTrackbarPos('S_MAX','settings')
		v_max = cv2.getTrackbarPos('V_MAX','settings')

		#print h_max

		# HSV
		#lower_red = np.array([24,30,94])
		#upper_red = np.array([26,64,81])

		lower = np.array([h_min,s_min,v_min])
		upper = np.array([h_max,s_max,v_max])
		
		mask = cv2.inRange(frame, lower, upper)

		cv2.imshow('mask',mask)

		res = cv2.bitwise_and(frame,frame, mask=mask)

		cv2.imshow('maskresult',res)

		res = cv2.cvtColor(res, cv2.COLOR_RGB2GRAY)
		res = cv2.adaptiveThreshold(res,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,11,2)
		cv2.imshow('maskmaskresult',res)

		contours,hierarchy = cv2.findContours(res,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE) 

		for cnt in contours:
			area = cv2.contourArea(cnt)
			perimeter = cv2.arcLength(cnt,True)
			if(perimeter > 0):
				vormfactor = 4 * math.pi * area / perimeter ** 2
				if area > 800:
					if vormfactor > 0.7:
						x,y,w,h = cv2.boundingRect(cnt)
						frame = cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,255),2)

		cv2.imshow('result', frame)

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break
	cap.release()
	cv2.destroyAllWindows()

ShowCamFeed()