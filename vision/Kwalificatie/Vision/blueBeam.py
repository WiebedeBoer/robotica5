import cv2
import numpy as np
import os, sys
import math
from decimal import *
import glob

def nothing(x):
	pass

def viewBeam():
	### DEBUG {
	cv2.namedWindow('settings', 2)
	cv2.createTrackbar('B','settings',0,100,nothing)
	cv2.createTrackbar('G','settings',0,100,nothing)
	cv2.createTrackbar('R','settings',0,100,nothing)

	cv2.createTrackbar('H_min','settings',0,255,nothing)
	cv2.createTrackbar('S_min','settings',0,255,nothing)
	cv2.createTrackbar('V_min','settings',0,255,nothing)

	cv2.createTrackbar('H_max','settings',0,255,nothing)
	cv2.createTrackbar('S_max','settings',0,255,nothing)
	cv2.createTrackbar('V_max','settings',0,255,nothing)

	cv2.createTrackbar('TH','settings',0,255,nothing)


	cv2.setTrackbarPos('H_min','settings',98)
	cv2.setTrackbarPos('S_min','settings',179)
	cv2.setTrackbarPos('V_min','settings',94)

	cv2.setTrackbarPos('H_max','settings',255)
	cv2.setTrackbarPos('S_max','settings',255)
	cv2.setTrackbarPos('V_max','settings',255)

	cv2.setTrackbarPos('TH','settings',90)

	### }


	# Text settings
	font = cv2.FONT_HERSHEY_SIMPLEX
	bottomLeftCornerOfText = (10,500)
	fontScale = 0.75
	fontColor = (255,255,255)
	lineType = 2
	#video capture
	cap = cv2.VideoCapture(0)
	xc, yc = 0, 0
	while(1):

		b = cv2.getTrackbarPos('B','settings')
		g = cv2.getTrackbarPos('G','settings')
		r = cv2.getTrackbarPos('R','settings')

		h_min = cv2.getTrackbarPos('H_min','settings')
		s_min = cv2.getTrackbarPos('S_min','settings')
		v_min = cv2.getTrackbarPos('V_min','settings')

		h_max = cv2.getTrackbarPos('H_max','settings')
		s_max = cv2.getTrackbarPos('S_max','settings')
		v_max = cv2.getTrackbarPos('V_max','settings')

		threshold = cv2.getTrackbarPos('TH','settings')
		#wait for socket
		#sok.recv(1024,0)

		_, frame = cap.read()
		blur = cv2.bilateralFilter(frame,17,75,75)

		blue = blur[:,:,0]

		cv2.imshow("blue", blue)

		ret,th = cv2.threshold(blue,threshold,255,cv2.THRESH_BINARY)
		filtered = cv2.bitwise_and(frame, frame, mask = th)
		filtered = cv2.cvtColor(filtered, cv2.COLOR_BGR2HSV)

		lower_blue = np.array([h_min,s_min,v_min])
		upper_blue = np.array([h_max,s_max,v_max])

		mask = cv2.inRange(filtered, lower_blue, upper_blue)
		res = cv2.bitwise_and(filtered, filtered, mask = mask)

		res = cv2.cvtColor(res, cv2.COLOR_HSV2BGR)
		gray_res = cv2.cvtColor(res, cv2.COLOR_BGR2GRAY)
		_,th = cv2.threshold(gray_res,1,255,cv2.THRESH_BINARY)

		cv2.imshow("filtered", th)

		contours, hierarchy = cv2.findContours(th, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
		for cnr in range(len(contours)):
			cnt = contours[cnr]
			area = cv2.contourArea(cnt)
			perimeter = cv2.arcLength(cnt, True)
			(x, y, w, h) = cv2.boundingRect(cnt)
			ar = w / float(h)
			approx = cv2.approxPolyDP(cnt, 0.04 * perimeter, True)
 
			# a square will have an aspect ratio that is approximately
			# equal to one, otherwise, the shape is a rectangle
			shape = "undefined"
			if len(approx) == 4 and area > 50:
				shape = "rectangle"

			#screen coordinates
			keypoints = (frame)
			if len(keypoints) == 1:
				xc = int(keypoints[0].pt[0])
				yc = int(keypoints[0].pt[1])

			if(shape == "rectangle"):
				#set contour around object
				frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)
				#calculate distance
				distance = calculateDistance(w)
				#apply text
				cv2.putText(frame,str(str(w)+ " " + str(h) + " " + str(round(distance, 2))),(x + (w / 2), y),font,fontScale,fontColor,lineType)
				#print command
				print("StopBeam;\n")
			# else:
			# 	print command
			# 	print("SearchBeam;\n")
		#show vid
		cv2.imshow("Frame", frame)
		#key
		if cv2.waitKey(1) & 0xFF == ord('q'):
			break
	cap.release()
	cv2.destroyAllWindows()

#calibrate distance
def calibration(w):
	# Initialize the known distance from the camera to the object
	KNOWN_DISTANCE = 10.0
	# Initialize the known object width, which in this case
	KNOWN_WIDTH = float(7.3)

	#return calculation
	return (KNOWN_DISTANCE * (w / 2)) / (KNOWN_WIDTH / 2)

#calculate distance
def calculateDistance(w):
	# Initialize the known distance from the camera to the object
	KNOWN_DISTANCE = 10.0
	# Initialize the known object width, which in this case
	KNOWN_WIDTH = Decimal(7.4)
	# Initialize the known object focal length, which in this case
	FOCAL_LENGTH = 476
	#return calculation
	w = Decimal(w)
	return ( FOCAL_LENGTH * (KNOWN_WIDTH / 2)) / (w / 2)

#beam function
viewBeam()
