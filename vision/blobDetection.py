import sys
import cv2
import numpy as np
import math
import time

class DetectEgg:

	def __init__(self, debug):
		self.setDefaultValues()
		self.debug = debug

		if debug:
			self.createTrackbars()
		self.DetectEgg()

	def nothing(self, x):
		pass

	def CLAHE(self, frame):
		clahe = cv2.createCLAHE(2.0, (8,8))
		cl = clahe.apply(frame)

		return cl

	def bluefilter(self, frame, RG):
		copy = frame.copy()
		copy[:,:,1] = copy[:,:,1]*(RG*0.01)
		copy[:,:,2] = copy[:,:,2]*(RG*0.01)
		
		return copy

	def createTrackbars(self):
		cv2.namedWindow('settings', 2)

		callback = self.nothing

		cv2.createTrackbar('H_MIN','settings',0,255,callback)
		cv2.createTrackbar('S_MIN','settings',0,255,callback)
		cv2.createTrackbar('V_MIN','settings',0,255,callback)

		cv2.createTrackbar('H_MAX','settings',0,255,callback)
		cv2.createTrackbar('S_MAX','settings',0,255,callback)
		cv2.createTrackbar('V_MAX','settings',0,255,callback)

		cv2.createTrackbar('AREA','settings',0,5000,callback)
		cv2.createTrackbar('CIRCULARITY','settings',0,100,callback)
		cv2.createTrackbar('CONVEXITY','settings',0,100,callback)

		cv2.createTrackbar('RG','settings',0,100,callback)

		cv2.createTrackbar('MIN_TH','settings',0,255,callback)
		cv2.createTrackbar('MAX_TH','settings',0,255,callback)

		cv2.createTrackbar('BLOCK_SIZE','settings',2,100,callback)

		cv2.setTrackbarPos('H_MIN','settings',self.h_min)
		cv2.setTrackbarPos('S_MIN','settings',self.s_min)
		cv2.setTrackbarPos('V_MIN','settings',self.v_min)

		cv2.setTrackbarPos('H_MAX','settings',self.h_max)
		cv2.setTrackbarPos('S_MAX','settings',self.s_max)
		cv2.setTrackbarPos('V_MAX','settings',self.v_max)

		cv2.setTrackbarPos('RG','settings',self.RG)

		cv2.setTrackbarPos('AREA','settings',self.area)
		cv2.setTrackbarPos('CIRCULARITY','settings',self.circularity)
		cv2.setTrackbarPos('CONVEXITY','settings',self.convexity)

		cv2.setTrackbarPos('MIN_TH','settings', self.min_th)
		cv2.setTrackbarPos('MAX_TH','settings', self.max_th)

	def setDefaultValues(self):

		# Set default variables
		self.h_min = 100
		self.s_min = 0
		self.v_min = 0

		self.h_max = 255
		self.s_max = 255
		self.v_max = 255

		self.area = 490
		self.circularity = 72
		self.convexity = 83

		self.RG = 90

		self.min_th = 1
		self.max_th = 255

		# Set params for blob detection
		self.params = cv2.SimpleBlobDetector_Params()
		self.params.filterByArea = True
		self.params.filterByCircularity = True
		self.params.filterByConvexity = True
		#self.params.filterByColor = True

		self.params.blobColor = 255
		self.params.minThreshold = self.min_th
		self.params.maxThreshold = self.max_th

		self.params.minArea = self.area
		self.params.minCircularity = self.circularity
		self.params.minConvexity = self.convexity

		#self.params = params

	def toPercentage(self, var):
		if var > 0:
			var = float(var) / 100
		return var

	def updateValues(self):
		self.h_min = cv2.getTrackbarPos('H_MIN','settings')
		self.s_min = cv2.getTrackbarPos('S_MIN','settings')
		self.v_min = cv2.getTrackbarPos('V_MIN','settings')

		self.h_max = cv2.getTrackbarPos('H_MAX','settings')
		self.s_max = cv2.getTrackbarPos('S_MAX','settings')
		self.v_max = cv2.getTrackbarPos('V_MAX','settings')

		self.area = cv2.getTrackbarPos('AREA','settings')
		self.circularity = self.toPercentage(cv2.getTrackbarPos('CIRCULARITY','settings'))

		self.convexity = self.toPercentage(cv2.getTrackbarPos('CONVEXITY','settings'))

		self.RG = cv2.getTrackbarPos('RG','settings')

		self.MIN_TH = cv2.getTrackbarPos('MIN_TH','settings')
		self.MAX_TH = cv2.getTrackbarPos('MAX_TH','settings')

		blockSize = cv2.getTrackbarPos('BLOCK_SIZE','settings')
		if blockSize % 2 != 0 and blockSize > 1:
			self.blockSize = blockSize

		self.COLOR_SENSITIVITY = cv2.getTrackbarPos('COLOR_SENSITIVITY','settings')

		self.params.blobColor = 255
		self.params.minThreshold = self.MIN_TH
		self.params.maxThreshold = self.MAX_TH

		self.params.minArea = self.area
		self.params.minCircularity = self.circularity
		self.params.minConvexity = self.convexity


	def DetectEgg(self):
		cap = cv2.VideoCapture(0)

		cap.read()
		time.sleep(1) # startup camera
				
		while True:

			if debug:
				self.updateValues()

			# Read the frame from the camera
			_, frame = cap.read()

			#improve contrast
			clahe_b = self.CLAHE(frame[:,:,0])

			denoise = cv2.fastNlMeansDenoising(clahe_b,None,10,5,10)
			#frame_blur = cv2.bilateralFilter(clahe_b,9,75,75)

			#cv2.imshow("blue channel blurred", frame_blur)
			#cv2.imshow("denoise", denoise)
			#cv2.imshow("blue channel", clahe_b)

			clahe = cv2.merge((clahe_b,frame[:,:,1], frame[:,:,2]))

			bluefiltered = self.bluefilter(clahe, self.RG)
			

			lower = np.array([self.h_min,self.s_min,self.v_min])
			upper = np.array([self.h_max,self.s_max,self.v_max])

			mask = cv2.inRange(bluefiltered, lower, upper)

			res = cv2.bitwise_and(bluefiltered,bluefiltered, mask=mask)

			# Set up the detector with default parameters.
			detector = cv2.SimpleBlobDetector_create(self.params)
			 
			# Detect blobs.
			keypoints = detector.detect(denoise)
			 
			# Draw detected blobs as red circles.
			keypoints_im = cv2.drawKeypoints(frame, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

			# Show keypoints
			cv2.imshow("output", keypoints_im)
			cv2.imshow("result", res)

			if not self.debug:
				break
			else:
				if cv2.waitKey(1) & 0xFF == ord('q'):
					break

debug = False
if str(sys.argv[1]) == "-d":
	debug = True

DetectEgg(debug)