import sys
import cv2
import numpy as np
import math
import time
from picamera.array import PiRGBArray
from picamera import PiCamera

class DetectEgg:

	# Distance calculating
	# Initialize the known distance from the camera to the object
	KNOWN_DISTANCE = 20.0
		# Initialize the known object width, which in this case
	KNOWN_WIDTH = 5.4
	 # Initialize the known object focal length, which in this case
	FOCAL_LENGTH = 395.684786196

	font                   = cv2.FONT_HERSHEY_SIMPLEX
	bottomLeftCornerOfText = (10,500)
	fontScale              = 0.75
	fontColor              = (255,255,255)
	lineType               = 2

	# Camera init
	camera = PiCamera()
	camera.resolution = (640, 480)
	camera.framerate = 30
	rawCapture = PiRGBArray(camera, size=(640, 480))

	time.sleep(0.1)

	def __init__(self, debug):
		self.debug = debug
		self.setDefaultValues()

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

		# Set the trackbars to the default values
		cv2.setTrackbarPos('H_MIN','settings',self.h_min)
		cv2.setTrackbarPos('S_MIN','settings',self.s_min)
		cv2.setTrackbarPos('V_MIN','settings',self.v_min)

		cv2.setTrackbarPos('H_MAX','settings',self.h_max)
		cv2.setTrackbarPos('S_MAX','settings',self.s_max)
		cv2.setTrackbarPos('V_MAX','settings',self.v_max)

		cv2.setTrackbarPos('RG','settings',self.RG)

		cv2.setTrackbarPos('AREA','settings',self.area)
		cv2.setTrackbarPos('CIRCULARITY','settings',int(100*self.circularity))
		cv2.setTrackbarPos('CONVEXITY','settings',int(100*self.convexity))

		cv2.setTrackbarPos('MIN_TH','settings', self.min_th)
		cv2.setTrackbarPos('MAX_TH','settings', self.max_th)

	def setDefaultValues(self):

		# Set default variables
		self.h_min = 80
		self.s_min = 0
		self.v_min = 0

		self.h_max = 255
		self.s_max = 255
		self.v_max = 255

		self.area = 490
		self.circularity = self.toPercentage(75)
		self.convexity = self.toPercentage(0)

		self.RG = 90

		self.min_th = 1
		self.max_th = 255

		# Set params for blob detection
		self.params = cv2.SimpleBlobDetector_Params()
		self.params.filterByArea = True
		self.params.filterByCircularity = True
		self.params.filterByConvexity = True
		self.params.filterByColor = True
		self.params.filterByInertia = True

		self.params.blobColor = 255
		self.params.maxArea = 200000000 #200.000.000

		self.params.minArea = self.area
		self.params.minCircularity = self.circularity
		self.params.minConvexity = self.convexity
		self.params.minThreshold = self.min_th
		self.params.maxThreshold = self.max_th

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

		self.min_th = cv2.getTrackbarPos('MIN_TH','settings')
		self.max_th = cv2.getTrackbarPos('MAX_TH','settings')

		blockSize = cv2.getTrackbarPos('BLOCK_SIZE','settings')
		if blockSize % 2 != 0 and blockSize > 1:
			self.blockSize = blockSize

		self.COLOR_SENSITIVITY = cv2.getTrackbarPos('COLOR_SENSITIVITY','settings')

		self.params.minThreshold = self.min_th
		self.params.maxThreshold = self.max_th
		self.params.minArea = self.area
		self.params.minCircularity = self.circularity
		self.params.minConvexity = self.convexity

	def calibration(self, w):
		return (self.KNOWN_DISTANCE * (w / 2)) / (self.KNOWN_WIDTH / 2)

	def calculateDistance(self, w):
		return (self.FOCAL_LENGTH * (self.KNOWN_WIDTH / 2)) / (w / 2)


	def DetectEgg(self):
		w = 640
		h = 480

		x, y = 0, 0

		count = 0
		eggDetectCount = 0

		for frame in camera.capture_continuous(rawCapture, format="bgr",  use_video_port=1):
			if self.debug:
				self.updateValues()
			else:
				count += 1

			# Read the frame from the camera
			frame = frame.array

			#improve contrast
			clahe_b = self.CLAHE(frame[:,:,0])
			clahe_g = self.CLAHE(frame[:,:,1])
			clahe_r = self.CLAHE(frame[:,:,2])

			cv2.imshow("clahe_b", clahe_b)

			denoise = cv2.medianBlur(clahe_b, 1)
			denoise = cv2.bilateralFilter(denoise,9,75,75)

			_,th = cv2.threshold(denoise,75,255,cv2.THRESH_BINARY)

			res = cv2.bitwise_and(denoise,denoise, mask=th)

			# Set up the detector with default parameters.
			detector = cv2.SimpleBlobDetector_create(self.params)

			# Detect blobs.
			keypoints = detector.detect(res)

			if len(keypoints) == 1:
				x = int(keypoints[0].pt[0])
				y = int(keypoints[0].pt[1])

				turn_x = x-(w/2)
				height_y = y-(h/2)

				width = (x+keypoints[0].size/2) - (x-keypoints[0].size/2)
				distance = self.calculateDistance(width)

				eggDetectCount += 1

				if self.debug:
					cv2.line(frame,(int(x-keypoints[0].size/2), y),(int(x+keypoints[0].size/2), y),(255,0,0),1)
					cv2.putText(frame,str(self.calculateDistance(width)), (int(x + (width / 2)), int(y)),\
						self.font,self.fontScale,self.fontColor,self.lineType)
					print str(turn_x) + " " + str(height_y) + " " + str(keypoints[0].size)

			if not self.debug:
				if eggDetectCount > 15:
					camera.start_preview()
					sleep(10)
					camera.stop_preview()
					print str(turn_x) + " " + str(height_y) + " " + str(distance)
					break
				elif count >= 30:
					camera.start_preview()
					sleep(10)
					camera.stop_preview()
					print "no egg was found."
					break

			else:
				keypoints_im = cv2.drawKeypoints(frame, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
				
				cv2.imshow("output", keypoints_im)


			if cv2.waitKey(1) & 0xFF == ord('q'):
				break


debug = False
if len(sys.argv) > 1:
	if sys.argv[1] == '-d':
		debug = True

def EggDetection(debug = False):
	DetectEgg(debug)
EggDetection(debug)