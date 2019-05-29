import sys

sys.path.append('Imp_Functions/')
sys.path.append('../pi/')

import cv2
import numpy as np
import math
from helpFunctions import *
from decimal import *
from clahe import CLAHE as cl


def viewBeam(frame):
    frame[:, :, 0] = cl(frame[:, :, 0])
    frame[:, :, 1] = cl(frame[:, :, 1])
    frame[:, :, 2] = cl(frame[:, :, 2])

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    lower_blue = np.array([100, 120, 50])
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
        approx = cv2.approxPolyDP(cnt, 0.02 * perimeter, True)

        # a square will have an aspect ratio that is approximately
        # equal to one, otherwise, the shape is a rectangle
        shape = "undefined"
        if (len(approx) >= 4 or len(approx) <= 6) and area > 100:
            shape = "rectangle"

        if shape == "rectangle":
            # calculate distance
            distance = calculateDistance(w, 625, 2.4)
            #distance = calculateDistance(w, 1320, 2.4) #old broad one
            #distance = calibration(w, 50, 2.4) #7200 #for calibration

            return round(distance, 2)


    return False
