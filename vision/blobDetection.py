import cv2
import numpy as np
import math
from matplotlib import pyplot as plt

def nothing(x):
	pass

def improveContrast(frame):
	hist, bins = np.histogram(frame.flatten(), 256, [0,256])

	cdf = hist.cumsum()
	cdf_normalized = cdf * hist.max()/ cdf.max()

	cdf_m = np.ma.masked_equal(cdf,0)
	cdf_m = (cdf_m - cdf_m.min())*255/(cdf_m.max()-cdf_m.min())
	cdf = np.ma.filled(cdf_m,0).astype('uint8')

	return cdf[frame]

def CLAHE(frame):
	clahe = cv2.createCLAHE(2.0, (8,8))
	cl = clahe.apply(frame)

	return cl

def bluefilter(frame, RG):
	copy = frame.copy()
	copy[:,:,1] = copy[:,:,1]*(RG*0.01)
	copy[:,:,2] = copy[:,:,2]*(RG*0.01)
	
	return copy

def createTrackbars():
	cv2.createTrackbar('MIN_EDGE','settings',1,500,nothing)
	cv2.createTrackbar('MAX_EDGE','settings',1,500,nothing)

	cv2.createTrackbar('H_MIN','settings',0,255,nothing)
	cv2.createTrackbar('S_MIN','settings',0,255,nothing)
	cv2.createTrackbar('V_MIN','settings',0,255,nothing)

	cv2.createTrackbar('H_MAX','settings',0,255,nothing)
	cv2.createTrackbar('S_MAX','settings',0,255,nothing)
	cv2.createTrackbar('V_MAX','settings',0,255,nothing)

	cv2.createTrackbar('RG','settings',0,100,nothing)

	cv2.createTrackbar('MIN_TH','settings',1,10,nothing)
	cv2.createTrackbar('MAX_TH','settings',0,255,nothing)

	cv2.createTrackbar('BLOCK_SIZE','settings',2,100,nothing)

	cv2.createTrackbar('COLOR_SENSITIVITY','settings',0,255,nothing)
	
	# Set default values
	cv2.setTrackbarPos('MIN_EDGE','settings',30)
	cv2.setTrackbarPos('MAX_EDGE','settings',215)
	
	cv2.setTrackbarPos('H_MIN','settings',100)
	cv2.setTrackbarPos('S_MIN','settings',0)
	cv2.setTrackbarPos('V_MIN','settings',0)

	cv2.setTrackbarPos('H_MAX','settings',255)
	cv2.setTrackbarPos('S_MAX','settings',255)
	cv2.setTrackbarPos('V_MAX','settings',255)

	cv2.setTrackbarPos('RG','settings',40)

	cv2.setTrackbarPos('MIN_TH','settings', 1)
	cv2.setTrackbarPos('MAX_TH','settings', 255)

	cv2.setTrackbarPos('BLOCK_SIZE','settings', 11)

	cv2.setTrackbarPos('COLOR_SENSITIVITY','settings', 255)



def DetectEgg():
	cap = cv2.VideoCapture(0)
	cv2.namedWindow('settings', 2)
	cv2.namedWindow('result', 2)

	# Create sliders 
	createTrackbars()

	params = cv2.SimpleBlobDetector_Params()

	# Filter by Area.
	params.filterByArea = True
	params.minArea = 100

	# Filter by Circularity
	params.filterByCircularity = True
	params.minCircularity = 0.8

	# Filter by Convexity
	params.filterByConvexity = True
	params.minConvexity = 0.9

	# Filter by color
	params.filterByColor = True
	

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

		RG = cv2.getTrackbarPos('RG','settings')

		MIN_TH = cv2.getTrackbarPos('MIN_TH','settings')
		MAX_TH = cv2.getTrackbarPos('MAX_TH','settings')

		if cv2.getTrackbarPos('BLOCK_SIZE','settings') % 2 != 0:
			blockSize = cv2.getTrackbarPos('BLOCK_SIZE','settings')

		COLOR_SENSITIVITY = cv2.getTrackbarPos('COLOR_SENSITIVITY','settings')

		params.blobColor = COLOR_SENSITIVITY
		params.minThreshold = MIN_TH
		params.maxThreshold = MAX_TH

		# Read the frame from the camera
		_, frame = cap.read()

		cv2.imshow("Regular Frame", frame)
		gray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
		gray_blur = cv2.GaussianBlur(gray, (9, 9), 0)

		original_th = cv2.adaptiveThreshold(gray_blur,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,11,2)
		
		frame_contrast = improveContrast(frame)
		frame_contrast = cv2.GaussianBlur(frame_contrast, (9, 9), 0)
		cv2.imshow("Improved contrast", frame_contrast)

		clahe_b = CLAHE(frame[:,:,0])
		clahe_g = CLAHE(frame[:,:,1])
		clahe_r = CLAHE(frame[:,:,2])

		cv2.imshow("clahe_b", clahe_b)
		cv2.imshow("clahe_g", clahe_g)
		cv2.imshow("clahe_r", clahe_r)

		clahe = cv2.merge((clahe_b,clahe_g,clahe_r))

		cv2.imshow("clahe", clahe)

		bluefiltered = bluefilter(frame, RG)
		bluefiltered_improved = bluefilter(clahe, RG)

		lower = np.array([h_min,s_min,v_min])
		upper = np.array([h_max,s_max,v_max])

		mask = cv2.inRange(bluefiltered, lower, upper)
		# canny = cv2.Canny(bluefiltered_improved, MIN_EDGE, MAX_EDGE, True)

		#,_ = cv2.findContours(canny,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
		# cv2.drawContours(canny, contours, -1, (255,0,0), 2)

		res = cv2.bitwise_and(clahe,clahe, mask=mask)		

		# = mask | canny
		#kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(3,3))
		#new_mask_open = cv2.morphologyEx(new_mask,cv2.MORPH_DILALATION,kernel)
		#new_mask_open = cv2.dilate(new_mask,kernel,iterations = 2)

		#res = cv2.bitwise_and(frame,frame, mask=new_mask_open)
		gray = cv2.cvtColor(res,cv2.COLOR_BGR2GRAY)
		th = cv2.adaptiveThreshold(gray,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,blockSize,MIN_TH)

		kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(3,3))
		#new_mask_open = cv2.morphologyEx(th,cv2.MORPH_DILALATION,kernel)
		new_mask_open = cv2.morphologyEx(th, cv2.MORPH_CLOSE, kernel)

		# res = cv2.bitwise_and(frame_contrast,frame_contrast, mask=th)	
		
		
		# for cnt in contours:
		# 	area = cv2.contourArea(cnt)
		# 	perimeter = cv2.arcLength(cnt,True)
		# 	if(perimeter > 0):
		# 		vormfactor = 4 * math.pi * area / perimeter ** 2
		# 		if area > 600:
		# 			if vormfactor > 0.85:
		# 				x,y,w,h = cv2.boundingRect(cnt)
		# 				res = cv2.rectangle(res,(x,y),(x+w,y+h),(255,0,255),2)

		# cv2.imshow("canny", canny)
		#cv2.imshow("new_mask", new_mask_open)
		cv2.imshow("res", res)
		cv2.imshow("I'm blue da ba dee da ba daa", bluefiltered)
		cv2.imshow("I'm blue da ba dee da ba daa_improved", bluefiltered_improved)
		cv2.imshow("th", th)
		cv2.imshow("original_th", original_th)
		cv2.imshow("new_mask_open",new_mask_open)


		#gray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
		#_,th = cv2.threshold(gray,200,255,cv2.THRESH_BINARY)

		# Set up the detector with default parameters.
		detector = cv2.SimpleBlobDetector_create(params)
		 
		# Detect blobs.
		keypoints = detector.detect(res)
		 
		# Draw detected blobs as red circles.
		# cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures the size of the circle corresponds to the size of blob
		im_with_keypoints = cv2.drawKeypoints(clahe, keypoints, np.array([]), (255,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

		contours,hierarchy = cv2.findContours(th,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE) 

		for cnt in contours:
			area = cv2.contourArea(cnt)
			perimeter = cv2.arcLength(cnt,True)
			if(perimeter > 0):
				vormfactor = 4 * math.pi * area / perimeter ** 2
				if area > 400:
					if vormfactor > 0.9:
						x,y,w,h = cv2.boundingRect(cnt)
						im_with_keypoints = cv2.rectangle(im_with_keypoints,(x,y),(x+w,y+h),(255,0,255),2)
					elif vormfactor > 0.8 and vormfactor <= 0.9:
						x,y,w,h = cv2.boundingRect(cnt)
						im_with_keypoints = cv2.rectangle(im_with_keypoints,(x,y),(x+w,y+h),(200,200,100),2)
		 
		# Show keypoints
		cv2.imshow("Keypoints", im_with_keypoints)

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break

DetectEgg()