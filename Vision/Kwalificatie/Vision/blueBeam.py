import sys

sys.path.append('Imp_Functions/')
sys.path.append('../pi/')

import cv2
import numpy as np
from helpFunctions import *


def viewBeam(frame):
	#b = CLAHE(frame[:, :, 0])
	#g = CLAHE(frame[:, :, 1])
	#r = CLAHE(frame[:, :, 2])

	#frame = cv2.merge((b,g,r))

	hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

	lower_blue = np.array([100, 150, 20])
	upper_blue = np.array([130, 255, 255])
	mask = cv2.inRange(hsv, lower_blue, upper_blue)

	res = cv2.bitwise_and(hsv, hsv, mask=mask)
	res = cv2.cvtColor(res, cv2.COLOR_HSV2BGR)

	gray_res = cv2.cvtColor(res, cv2.COLOR_BGR2GRAY)
	_, th = cv2.threshold(gray_res, 1, 255, cv2.THRESH_BINARY)
	contours, hierarchy = cv2.findContours(th, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
	for cnt in contours:
		area = cv2.contourArea(cnt)
		perimeter = cv2.arcLength(cnt, True)
		(x, y, w, h) = cv2.boundingRect(cnt)
		approx = cv2.approxPolyDP(cnt, (0.02 * perimeter), True)

		# a square will have an aspect ratio that is approximately
		# equal to one, otherwise, the shape is a rectangle
		if len(approx) >= 4 and len(approx) <= 6 and area > 200:
			#print("Draw circle")
			#print("approx length: ", len(approx))
			#print("area: ", area)
			# calculate distance
			cv2.circle(res, (x + (w /2),y + (h/2)), 5, (0, 0, 255), -1)
			distance = calculateDistance(w, 625, 2.4)
			xyAxis = whichDirection((x + w) / 2, (y + h) / 2)
			#return str(str(distance) + ':' + str(xyAxis[0]) + ':' + str(xyAxis[1]))
	#cv2.imshow("res", res)
	return False
