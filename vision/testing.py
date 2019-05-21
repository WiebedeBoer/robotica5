import sys

sys.path.append('Imp_Functions/')
sys.path.append('../pi/')

import cv2
import numpy as np
import math
from helpFunctions import *
from camera_opencv import getCapture
from decimal import *


def viewBeam():
    # video capture
    cap = getCapture()
    xc, yc = 0, 0

    _, frame = cap.read()
    blur = cv2.bilateralFilter(frame, 17, 75, 75)

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    lower_blue = np.array([100, 120, 90])
    upper_blue = np.array([107, 255, 255])
    mask = cv2.inRange(hsv, lower_blue, upper_blue)

    res = cv2.bitwise_and(hsv, hsv, mask=mask)

    res = cv2.cvtColor(res, cv2.COLOR_HSV2BGR)
    gray_res = cv2.cvtColor(res, cv2.COLOR_BGR2GRAY)
    _, th = cv2.threshold(gray_res, 1, 255, cv2.THRESH_BINARY)

    contours, hierarchy = cv2.findContours(th, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    for cnr in range(len(contours)):
        cnt = contours[cnr]
        area = cv2.contourArea(cnt)
        perimeter = cv2.arcLength(cnt, True)
        (x, y, w, h) = cv2.boundingRect(cnt)
        ar = w / float(h)
        approx = cv2.approxPolyDP(cnt, 0.02 * perimeter, True)

        # a square will have an aspect ratio that is approximately
        # equal to one, otherwise, the shape is a rectangle
        shape = "undefined"
        if (len(approx) >= 4 or len(approx) <= 6) and area > 100:
            shape = "rectangle"

        if (shape == "rectangle"):
            # set contour around object
            frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)
            # calculate distance
            distance = calculateDistance(w)
            # apply text
            return str(w) + " " + str(h) + " " + str(round(distance, 2))

        else:
            return "Nothing"
