import sys
sys.path.append('Imp_Functions/')
sys.path.append('Wedstrijd/Eggtelligence/')

import cv2
from fm_ORB import fm_ORB
from eggDetection import DetectEgg
from helpFunctions import *
from qrReader import QRReader


def eggtelligence(frame, argument, argument1):
	switcher = {
		0: eggDistance,
		1: qrDistance,
		2: chicken
	}
	func = switcher.get(argument, "Something went wrong")
	return func(frame, argument1)   # Returns distance, x-axis, y-axis


def eggDistance(frame, argument):
	closestDistance = []
	detectEgg = DetectEgg(frame, None)
	eggDetected = detectEgg.DetectEgg()
	xyAxis = []

	if eggDetected != False:
		allKeypointsWidths = detectEgg.widthKeypoints(eggDetected)

		for idx, width in enumerate(allKeypointsWidths):
			distance = calculateDistance(width, 420, 5.4)
			if distance < closestDistance:
				closestDistance = distance 
				xyAxis = whichDirection(eggDetected[idx].pt[0], eggDetected[idx].pt[1])
		return str(str(closestDistance) + ':' + str(xyAxis[0]) + ':' + str(xyAxis[1]))

	else:
		return False


def qrDistance(frame, argument):
	findQRCode = townSwitcher(argument)
	qrReader = QRReader(frame)
	qr = qrReader.findQR(findQRCode)

	if qr != None:
		x, y, w, h = qrReader.getRectangleQR()
		distance = calculateDistance(w, 114, 150)
		xyAxis = whichDirection(x + (w / 2), y + (h / 2))
		return str(str(distance) + ':' + str(xyAxis[0]) + ':' + str(xyAxis[1]))
	else:
		return False


def chicken(frame, argument):
	imgTrainColor = cv2.imread('Images/TrainImg/Kip/piKip.jpg')
	# try:
	# 	from camera_pi import Camera_pi
	# 	screenWidth, screenHeight = Camera_pi.getSettings()
	# except:
	# 	from camera_opencv import Camera_opencv
	# 	screenWidth, screenHeight = Camera_opencv.getSettings()

	# imgTrainColor = cv2.resize(imgTrainColor, (screenWidth, screenHeight))
	rectanglePts = fm_ORB(frame, imgTrainColor, 60)

	if rectanglePts != False and rectanglePts != None:
		center = calculateCenter(rectanglePts)
		xyAxis = whichDirection(center[0], center[1])
		return str("Unknown" + ':' + str(xyAxis[0]) + ':' + str(xyAxis[1]))
	else:
		return False


def townSwitcher(argument):
	switcher = {
		0: "'s-Hertogenbosch",
		1: "Eindhoven",
		2: "Eibergen",
		3: "Barneveld",
		4: "Duckstad"
	}
	func = switcher.get(argument, "Something went wrong")
	return func


