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

	cv2.createTrackbar('ALPHA','settings',0,10,nothing)
	cv2.createTrackbar('BETA','settings',1,100,nothing)

	cv2.setTrackbarPos('H_MAX','settings',255)
	cv2.setTrackbarPos('S_MAX','settings',255)
	cv2.setTrackbarPos('V_MAX','settings',255)
	cv2.setTrackbarPos('ALPHA','settings',1)

	while(True):
		_, frame = cap.read()

		cv2.imshow('default',frame)
		value = cv2.getTrackbarPos('THRESH','settings')
		alpha = cv2.getTrackbarPos('ALPHA','settings')
		beta = cv2.getTrackbarPos('BETA','settings')
		
		frame = cv2.convertScaleAbs(frame, alpha=alpha, beta=beta)
		cv2.imshow('Bright',frame)
		#frame = cv2.bilateralFilter(frame,20,100,100)
		threshold = cv2.getTrackbarPos('THRESH','settings')

		h_min = cv2.getTrackbarPos('H_MIN','settings')
		s_min = cv2.getTrackbarPos('S_MIN','settings')
		v_min = cv2.getTrackbarPos('V_MIN','settings')

		h_max = cv2.getTrackbarPos('H_MAX','settings')
		s_max = cv2.getTrackbarPos('S_MAX','settings')
		v_max = cv2.getTrackbarPos('V_MAX','settings')

		lower = np.array([h_min,s_min,v_min])
		upper = np.array([h_max,s_max,v_max])
		
		mask = cv2.inRange(frame, lower, upper)

		res = cv2.bitwise_and(frame,frame, mask=mask)

		cv2.imshow('maskresult',res)
		
		res = cv2.cvtColor(res, cv2.COLOR_RGB2GRAY)	

		ret, th = cv2.threshold(res,threshold,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)


		# noise removal
		kernel = np.ones((3,3),np.uint8)
		opening = cv2.morphologyEx(th,cv2.MORPH_OPEN,kernel, iterations=3)
		
		cv2.imshow('!!!',opening)

		# sure background area
		sure_bg = cv2.dilate(opening,kernel,iterations=2)

		# Finding sure foreground area
		dist_transform = cv2.distanceTransform(opening,cv2.DIST_L2,5)
		ret, sure_fg = cv2.threshold(dist_transform,0.7*dist_transform.max(),255,0)

		# Finding unknown region
		sure_fg = np.uint8(sure_fg)
		unknown = cv2.subtract(sure_bg,sure_fg)

		cv2.imshow('sure',sure_fg)
		cv2.imshow('not sure',unknown)

		# Marker labelling
		ret, markers = cv2.connectedComponents(sure_fg)

		# Add one to all labels so that sure background is not 0, but 1
		markers = markers+1

		# Now, mark the region of unknown with zero
		markers[unknown==255] = 0

		markers = cv2.watershed(frame,markers)
		frame[markers == -1] = [255,0,0]


		contours,hierarchy = cv2.findContours(res,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE) 

		for cnt in contours:
			area = cv2.contourArea(cnt)
			perimeter = cv2.arcLength(cnt,True)
			if(perimeter > 0):
				vormfactor = 4 * math.pi * area / perimeter ** 2
				if area > 800:
					if vormfactor > 0.85:
						x,y,w,h = cv2.boundingRect(cnt)
						frame = cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,255),2)
					elif vormfactor > 0.7 and vormfactor <= 0.85:
						x,y,w,h = cv2.boundingRect(cnt)
						frame = cv2.rectangle(frame,(x,y),(x+w,y+h),(200,200,100),2)

		cv2.imshow('result', frame)

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break
	cap.release()
	cv2.destroyAllWindows()

ShowCamFeed()