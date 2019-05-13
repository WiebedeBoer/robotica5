import cv2
import numpy as np
import math
from matplotlib import pyplot as plt

def nothing(x):
	pass

def auto_canny(image, sigma=0.33):
	# compute the median of the single channel pixel intensities
	v = np.median(image)
 
	# apply automatic Canny edge detection using the computed median
	lower = int(max(0, (1.0 - sigma) * v))
	upper = int(min(255, (1.0 + sigma) * v))
	edged = cv2.Canny(image, lower, upper, True)
 
	# return the edged image
	return edged

def DetectEgg():
	cap = cv2.VideoCapture(0)
	cv2.namedWindow('settings', 2)
	cv2.namedWindow('result', 2)
	cv2.createTrackbar('MIN_EDGE','settings',1,500,nothing)
	cv2.createTrackbar('MAX_EDGE','settings',1,500,nothing)

	cv2.setTrackbarPos('MIN_EDGE','settings',110)
	cv2.setTrackbarPos('MAX_EDGE','settings',146)

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

		# Read the values from the sliders
		MIN_EDGE = cv2.getTrackbarPos('MIN_EDGE','settings')
		MAX_EDGE = cv2.getTrackbarPos('MAX_EDGE','settings')

		h_min = cv2.getTrackbarPos('H_MIN','settings')
		s_min = cv2.getTrackbarPos('S_MIN','settings')
		v_min = cv2.getTrackbarPos('V_MIN','settings')

		h_max = cv2.getTrackbarPos('H_MAX','settings')
		s_max = cv2.getTrackbarPos('S_MAX','settings')
		v_max = cv2.getTrackbarPos('V_MAX','settings')

		# Read the frame from the camera
		_, frame = cap.read()

		# Remove black bars
		gray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
		_,thresh = cv2.threshold(gray,1,255,cv2.THRESH_BINARY)

		contours,hierarchy = cv2.findContours(thresh,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
		cnt = contours[0]
		x,y,w,h = cv2.boundingRect(cnt)

		frame = frame[y:y+h,x:x+w]

		#frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

		# Create our shapening kernel, it must equal to one eventually
		#kernel_sharpening = np.array([[-1,-1,-1], [-1,9,-1],[-1,-1,-1]])

		# applying the sharpening kernel to the input image & displaying it.
		#sharpened = cv2.filter2D(frame, -1, kernel_sharpening)
		#cv2.imshow('SHARPENED',sharpened)

		frame = cv2.GaussianBlur(frame, (5, 5), 0)
		frame_canny = cv2.Canny(frame,MIN_EDGE,MAX_EDGE,True)


		kernel = np.ones((3,3),np.uint8)
		frame_canny = cv2.morphologyEx(frame_canny, cv2.MORPH_GRADIENT, kernel)

		cv2.imshow('CANNY',frame_canny)

		th, im_th = cv2.threshold(frame_canny, 220, 255, cv2.THRESH_BINARY);
 
		# Copy the thresholded image.
		im_floodfill = im_th.copy()
		 
		# Mask used to flood filling.
		# Notice the size needs to be 2 pixels than the image.
		h, w = im_th.shape[:2]
		mask = np.zeros((h+2, w+2), np.uint8)
		 
		# Floodfill from point (0, 0)
		cv2.imshow('BEFORE',im_floodfill)
		(x, y, width, height) = cv2.getWindowImageRect('BEFORE')

		print "width :"+str(width)+", height :"+str(height)+", x :"+str(x)+", y :"+str(y)

		# cv2.floodFill(im_floodfill, mask, (0,0), 255)
		# cv2.floodFill(im_floodfill, mask, (width-1,0), 255)
		# cv2.floodFill(im_floodfill, mask, (0,height-1), 255)
		# cv2.floodFill(im_floodfill, mask, (width-1,height-1), 255)

		#res = cv2.bitwise_and(frame,frame, mask=im_floodfill)

		

		# lower = np.array([h_min,s_min,v_min])
		# upper = np.array([h_max,s_max,v_max])

		# mask = cv2.inRange(res, lower, upper)

		# res = cv2.bitwise_and(res,res, mask=mask)
		
		# #res = cv2.dilate(res,kernel,iterations = 1)
		

		# th, im_th = cv2.threshold(res, 20, 255, cv2.THRESH_BINARY_INV);

	 
		# # Invert floodfilled image
		# im_floodfill_inv = cv2.bitwise_not(im_floodfill)
		 
		# # Combine the two images to get the foreground.
		# im_out = im_th | im_floodfill_inv
		 
		# # Display images.
		# cv2.imshow("Thresholded Image", im_th)
		# cv2.imshow("Floodfilled Image", im_floodfill)
		# cv2.imshow("Inverted Floodfilled Image", im_floodfill_inv)
		# cv2.imshow("Foreground", im_out)



		# #frame_sharp = cv2.Canny(sharpened,MIN_EDGE,MIN_EDGE*3,3)
		# #frame_sharp_blurred = cv2.bilateralFilter(frame_sharp,90,100,100)
		# #frame = cv2.applyColorMap(frame, cv2.COLORMAP_JET)
		# #cv2.imshow('DEF_CANNY',frame_canny)
		# cv2.imshow('SHRP_BLURRED_CANNY',res)

		# im_floodfill = cv2.cvtColor(im_floodfill, cv2.COLOR_BGR2GRAY)
		# contours,hierarchy = cv2.findContours(im_floodfill,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE) 

		# for cnt in contours:
		# 	area = cv2.contourArea(cnt)
		# 	perimeter = cv2.arcLength(cnt,True)
		# 	if(perimeter > 0):
		# 		vormfactor = 4 * math.pi * area / perimeter ** 2
		# 		if area > 600:
		# 			if vormfactor > 0.85:
		# 				x,y,w,h = cv2.boundingRect(cnt)
		# 				frame = cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,255),2)
		# 			elif vormfactor > 0.7 and vormfactor <= 0.85:
		# 				x,y,w,h = cv2.boundingRect(cnt)
		# 				frame = cv2.rectangle(frame,(x,y),(x+w,y+h),(200,200,100),2)

		cv2.imshow('result', res)

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break

DetectEgg()