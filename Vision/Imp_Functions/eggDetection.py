import cv2
import sys
import numpy as np
from helpFunctions import *


class DetectEgg:
	font = cv2.FONT_HERSHEY_SIMPLEX
	bottomLeftCornerOfText = (10, 500)
	fontScale = 0.75
	fontColor = (255, 255, 255)
	lineType = 2

	def __init__(self, frame, debug):
		self.frame = frame
		self.debug = debug
		self.setDefaultValues()

	def nothing(self, x):
		pass

	def bluefilter(self, frame, RG):
		copy = frame.copy()
		copy[:, :, 1] = copy[:, :, 1] * (RG * 0.01)
		copy[:, :, 2] = copy[:, :, 2] * (RG * 0.01)

		return copy

	def createTrackbars(self):
		cv2.namedWindow('settings', 2)

		callback = self.nothing

		cv2.createTrackbar('H_MIN', 'settings', 0, 255, callback)
		cv2.createTrackbar('S_MIN', 'settings', 0, 255, callback)
		cv2.createTrackbar('V_MIN', 'settings', 0, 255, callback)

		cv2.createTrackbar('H_MAX', 'settings', 0, 255, callback)
		cv2.createTrackbar('S_MAX', 'settings', 0, 255, callback)
		cv2.createTrackbar('V_MAX', 'settings', 0, 255, callback)

		cv2.createTrackbar('AREA', 'settings', 0, 5000, callback)
		cv2.createTrackbar('CIRCULARITY', 'settings', 0, 100, callback)
		cv2.createTrackbar('CONVEXITY', 'settings', 0, 100, callback)

		cv2.createTrackbar('RG', 'settings', 0, 100, callback)

		cv2.createTrackbar('MIN_TH', 'settings', 0, 255, callback)
		cv2.createTrackbar('MAX_TH', 'settings', 0, 255, callback)

		cv2.createTrackbar('BLOCK_SIZE', 'settings', 2, 100, callback)

		# Set the trackbars to the default values
		cv2.setTrackbarPos('H_MIN', 'settings', self.h_min)
		cv2.setTrackbarPos('S_MIN', 'settings', self.s_min)
		cv2.setTrackbarPos('V_MIN', 'settings', self.v_min)

		cv2.setTrackbarPos('H_MAX', 'settings', self.h_max)
		cv2.setTrackbarPos('S_MAX', 'settings', self.s_max)
		cv2.setTrackbarPos('V_MAX', 'settings', self.v_max)

		cv2.setTrackbarPos('RG', 'settings', self.RG)

		cv2.setTrackbarPos('AREA', 'settings', self.area)
		cv2.setTrackbarPos('CIRCULARITY', 'settings', int(100 * self.circularity))
		cv2.setTrackbarPos('CONVEXITY', 'settings', int(100 * self.convexity))

		cv2.setTrackbarPos('MIN_TH', 'settings', self.min_th)
		cv2.setTrackbarPos('MAX_TH', 'settings', self.max_th)

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
		self.params.maxArea = 200000000  # 200.000.000

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
		self.h_min = cv2.getTrackbarPos('H_MIN', 'settings')
		self.s_min = cv2.getTrackbarPos('S_MIN', 'settings')
		self.v_min = cv2.getTrackbarPos('V_MIN', 'settings')

		self.h_max = cv2.getTrackbarPos('H_MAX', 'settings')
		self.s_max = cv2.getTrackbarPos('S_MAX', 'settings')
		self.v_max = cv2.getTrackbarPos('V_MAX', 'settings')

		self.area = cv2.getTrackbarPos('AREA', 'settings')
		self.circularity = self.toPercentage(cv2.getTrackbarPos('CIRCULARITY', 'settings'))
		self.convexity = self.toPercentage(cv2.getTrackbarPos('CONVEXITY', 'settings'))

		self.RG = cv2.getTrackbarPos('RG', 'settings')

		self.min_th = cv2.getTrackbarPos('MIN_TH', 'settings')
		self.max_th = cv2.getTrackbarPos('MAX_TH', 'settings')

		blockSize = cv2.getTrackbarPos('BLOCK_SIZE', 'settings')
		if blockSize % 2 != 0 and blockSize > 1:
			self.blockSize = blockSize

		self.COLOR_SENSITIVITY = cv2.getTrackbarPos('COLOR_SENSITIVITY', 'settings')

		self.params.minThreshold = self.min_th
		self.params.maxThreshold = self.max_th
		self.params.minArea = self.area
		self.params.minCircularity = self.circularity
		self.params.minConvexity = self.convexity

	def widthKeypoints(self, keypoints):
		allWidths = []
		for kp in keypoints:
			x = int(kp.pt[0])
			width = (x + kp.size / 2) - (x - kp.size / 2)
			allWidths.append(width)

		return allWidths

	def DetectEgg(self):
		w = 640
		h = 480

		x, y = 0, 0

		count = 0
		eggDetectCount = 0

		# improve contrast
		clahe_b = CLAHE(self.frame[:, :, 0])

		denoise = cv2.medianBlur(clahe_b, 1)
		denoise = cv2.bilateralFilter(denoise, 9, 75, 75)

		_, th = cv2.threshold(denoise, 75, 255, cv2.THRESH_BINARY)

		res = cv2.bitwise_and(denoise, denoise, mask=th)

		# Set up the detector with default parameters.
		detector = cv2.SimpleBlobDetector_create(self.params)

		# Detect blobs.
		keypoints = detector.detect(res)

		if len(keypoints) <= 5:
			for kp in keypoints:
				eggDetectCount += 1


		if eggDetectCount > 0:
			return keypoints
		return False
