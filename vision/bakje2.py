import numpy as np
import cv2
import math
import random

def nothing():
	pass

def ShowCamFeed():
	cap = cv2.VideoCapture(0)

	while(True):
		_, frame = cap.read()

		cv2.imshow('default',frame)
		frame = cv2.bilateralFilter(frame,9,50,50)
		gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
		#gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
		#hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
		hsv = cv2.cvtColor(frame, cv2.COLOR_RGB2HSV)
                #lower_red = np.array([0,50,50])
                #upper_red = np.array([10,255,255])
                redmask = cv2.inRange(hsv, lower_red, upper_red)

		# #print h_max

		# # HSV
		# lower_red = np.array([24,30,94])
		# upper_red = np.array([26,64,81])

		# lower_first = np.array([h_min,s_min,v_min])
		# upper_first = np.array([h_max,s_max,v_max])

		# #lower_last = np.array([h_min_after,s_min_after,v_min_after])
		# #upper_last = np.array([h_max_after,s_max_after,v_max_after])
		
		# mask_first = cv2.inRange(frame, lower_first, upper_first)
		# #mask_last = cv2.inRange(frame, lower_last, upper_last)
		# mask = mask_first# | mask_last
		# cv2.imshow('mask',mask)

		# res = cv2.bitwise_and(frame,frame, mask=mask)
		# cv2.imshow('maskresult',res)
		# res = cv2.cvtColor(res, cv2.COLOR_RGB2GRAY)

		lower_red = np.array([24,30,94])
		upper_red = np.array([26,64,81])

		lower_yellow = np.array([65,58,5])
		upper_yellow = np.array([71,68,25])

		lower_blue = np.array([62,65,68])
		upper_blue = np.array([255,255,255])

		lower_green = np.array([62,65,68])
		upper_green = np.array([255,255,255])

                _,res = cv2.threshold(hsv,215,255,cv2.THRESH_BINARY)
		#_,res = cv2.threshold(gray,215,255,cv2.THRESH_BINARY)
		cv2.imshow('Trunc',res)
		#res = cv2.adaptiveThreshold(res,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,11,1)
		#cv2.imshow('Gaussian',res)

                yellowmask = cv2.inRange(hsv, lower_yellow, upper_yellow)
		contours, hierarchy = cv2.findContours(yellowmask,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

		cv2.imshow('frame',yellowmask)

		#contours,hierarchy = cv2.findContours(res,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE) 

		for cnt in contours:
			area = cv2.contourArea(cnt)
			perimeter = cv2.arcLength(cnt,True)
			if(perimeter > 0):
				vormfactor = 4 * math.pi * area / perimeter ** 2
				if area > 800:
					if vormfactor > 0.89:
						x,y,w,h = cv2.boundingRect(cnt)
						frame = cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,255),2)
					elif vormfactor > 0.85 and vormfactor <= 0.89:
						x,y,w,h = cv2.boundingRect(cnt)
						frame = cv2.rectangle(frame,(x,y),(x+w,y+h),(255,255,0),2)

		cv2.imshow('result', frame)

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break
	cap.release()
	cv2.destroyAllWindows()

ShowCamFeed()
