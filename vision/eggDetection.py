import cv2
import numpy as np
import math
from matplotlib import pyplot as plt

def nothing(x):
	pass

def DetectEgg():
	cap = cv2.VideoCapture(0)
	cv2.namedWindow('settings', 2)
	cv2.namedWindow('result', 2)
	cv2.createTrackbar('MIN_EDGE','settings',1,255,nothing)

	cv2.setTrackbarPos('MIN_EDGE','settings',100)

	cv2.createTrackbar('H_MIN','settings',0,255,nothing)
	cv2.createTrackbar('S_MIN','settings',0,255,nothing)
	cv2.createTrackbar('V_MIN','settings',0,255,nothing)

	cv2.createTrackbar('H_MAX','settings',0,255,nothing)
	cv2.createTrackbar('S_MAX','settings',0,255,nothing)
	cv2.createTrackbar('V_MAX','settings',0,255,nothing)

	cv2.setTrackbarPos('H_MAX','settings',255)
	cv2.setTrackbarPos('S_MAX','settings',255)
	cv2.setTrackbarPos('V_MAX','settings',255)
	cv2.setTrackbarPos('S_MIN','settings',140)

	while(True):
		MIN_EDGE = cv2.getTrackbarPos('MIN_EDGE','settings')

		_, frame = cap.read()
		#frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)


		# Create our shapening kernel, it must equal to one eventually
		kernel_sharpening = np.array([[-1,-1,-1], [-1,9,-1],[-1,-1,-1]])

		# applying the sharpening kernel to the input image & displaying it.
		sharpened = cv2.filter2D(frame, -1, kernel_sharpening)
		cv2.imshow('SHARPENED',sharpened)


		frame_def = cv2.Canny(frame,MIN_EDGE,MIN_EDGE*3,3)
		cv2.imshow('CANNY',frame_def)

		frame_shrp = cv2.Canny(sharpened,MIN_EDGE,MIN_EDGE*3,3)
		cv2.imshow('CANNY_SHARP',frame_shrp)


		res = cv2.bitwise_and(frame,frame, mask=frame_shrp)

		h_min = cv2.getTrackbarPos('H_MIN','settings')
		s_min = cv2.getTrackbarPos('S_MIN','settings')
		v_min = cv2.getTrackbarPos('V_MIN','settings')

		h_max = cv2.getTrackbarPos('H_MAX','settings')
		s_max = cv2.getTrackbarPos('S_MAX','settings')
		v_max = cv2.getTrackbarPos('V_MAX','settings')

		lower = np.array([h_min,s_min,v_min])
		upper = np.array([h_max,s_max,v_max])

		mask = cv2.inRange(frame, lower, upper)

		res = cv2.bitwise_and(res,res, mask=mask)
		kernel = np.ones((2,2),np.uint8)
		#res = cv2.dilate(res,kernel,iterations = 1)
		res = cv2.morphologyEx(res, cv2.MORPH_CLOSE, kernel)

		th, im_th = cv2.threshold(res, 20, 255, cv2.THRESH_BINARY_INV);
	 
		# Copy the thresholded image.
		im_floodfill = im_th.copy()

		h, w = im_th.shape[:2]
		mask = np.zeros((h+2, w+2), np.uint8)

		(x, y, w, h) = cv2.getWindowImageRect('result')

		
		#print "w = "+str(w)+", h = "+str(h)

		cv2.floodFill(im_floodfill, mask, (0,25), 0)
		cv2.floodFill(im_floodfill, mask, (100,25), 0)
		cv2.floodFill(im_floodfill, mask, (0,75), 0)
		cv2.floodFill(im_floodfill, mask, (100,75), 0)

	 
		# Invert floodfilled image
		im_floodfill_inv = cv2.bitwise_not(im_floodfill)
		 
		# Combine the two images to get the foreground.
		im_out = im_th | im_floodfill_inv
		 
		# Display images.
		cv2.imshow("Thresholded Image", im_th)
		cv2.imshow("Floodfilled Image", im_floodfill)
		cv2.imshow("Inverted Floodfilled Image", im_floodfill_inv)
		cv2.imshow("Foreground", im_out)



		#frame_sharp = cv2.Canny(sharpened,MIN_EDGE,MIN_EDGE*3,3)
		#frame_sharp_blurred = cv2.bilateralFilter(frame_sharp,90,100,100)
		#frame = cv2.applyColorMap(frame, cv2.COLORMAP_JET)
		#cv2.imshow('DEF_CANNY',frame_def)
		cv2.imshow('SHRP_BLURRED_CANNY',res)

		im_floodfill = cv2.cvtColor(im_floodfill, cv2.COLOR_BGR2GRAY)
		contours,hierarchy = cv2.findContours(im_floodfill,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE) 

		for cnt in contours:
			area = cv2.contourArea(cnt)
			perimeter = cv2.arcLength(cnt,True)
			if(perimeter > 0):
				vormfactor = 4 * math.pi * area / perimeter ** 2
				if area > 600:
					if vormfactor > 0.85:
						x,y,w,h = cv2.boundingRect(cnt)
						frame = cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,255),2)
					elif vormfactor > 0.7 and vormfactor <= 0.85:
						x,y,w,h = cv2.boundingRect(cnt)
						frame = cv2.rectangle(frame,(x,y),(x+w,y+h),(200,200,100),2)

			cv2.imshow('result', frame)

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break

DetectEgg()