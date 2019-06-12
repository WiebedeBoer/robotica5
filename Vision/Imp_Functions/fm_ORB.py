import sys
sys.path.append('../')

import numpy as np
import cv2
from helpFunctions import *


# Calculates the distance between points and the center
def calculateDistance(pts, center):
    allXDistances = []
    allYDistances = []

    for p in pts:
        allXDistances.append((abs(p[0][0]) - center[0]))
        allYDistances.append((abs(p[0][1]) - center[0]))

    sumX = 0
    for x in allXDistances:
        sumX += x

    sumY = 0
    for y in allYDistances:
        sumY += y
    return [sumX / len(pts), sumY / len(pts)]


# Go through all the points and seek the points to put the rectangle around
def findRectanglePoints(pts):
    minXmaxY = None
    maxXminY = None

    for p in pts:
        if minXmaxY is None or maxXminY is None:
            minXmaxY = [p[0][0], p[0][1]]
            maxXminY = [p[0][0], p[0][1]]

        if p[0][0] < minXmaxY[0]: minXmaxY[0] = p[0][0]
        if p[0][0] > maxXminY[0]: maxXminY[0] = p[0][0]

        if p[0][1] < maxXminY[1]: maxXminY[1] = p[0][1]
        if p[0][1] > minXmaxY[1]: minXmaxY[1] = p[0][1]
    return [minXmaxY, maxXminY]


def fm_ORB(frame, imgTrainColor, MIN_MATCH):
    imgCamColor = frame

    # Create the orb instance
    try:
        orb = cv2.ORB_create()  # is being used for local
    except:
        orb = cv2.ORB()  # Is being used for pi

    # Create a Brute Force Matcher with the algorithm NORM_HAMMING and crossCheck
    bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)

    imgTrainGray = cv2.cvtColor(imgTrainColor, cv2.COLOR_BGR2GRAY)

    kpTrain = orb.detect(imgTrainGray, None)   # Detect orbs in imgTrainGray
    kpTrain, desTrain = orb.compute(imgTrainGray, kpTrain)    # Detect descriptors in imgTrainGray

    firstTime = True    # If firstTime is True then set width and heights of frame and images as variables
    MIN_MATCH_COUNT = MIN_MATCH    # There need to be at least the given amound of matches before drawing a rectangle

    imgCamGray = cv2.cvtColor(imgCamColor, cv2.COLOR_BGR2GRAY)
    imgCamColor = CLAHE(imgCamGray)  # Sets the contrast of the frame

    kpCam = orb.detect(imgCamGray, None)    # Detect orbs in imgTrainGray
    kpCam, desCam = orb.compute(imgCamGray, kpCam)  # Detect descriptors in imgTrainGray
    matches = bf.match(desCam, desTrain)    # Check for matches between the two descriptors images

    allGood_matches = []    # If the mark is under a special ratio it is appended in this list
    best_matches = []   # Only an amount of the best good matches are appended in this list

    # Get shape and dimensions of the image and frame
    if firstTime is True:
        h1, w1 = imgCamColor.shape[:2]
        h2, w2 = imgTrainColor.shape[:2]
        nWidth = w1 + w2
        nHeight = max(h1, h2)
        hdif = (h1 - h2) / 2
        firstTime = False

    # For every match check if the match is good enough
    for m in matches:
        if (m.distance / 100) < 0.6:
            allGood_matches.append(m)

    allGood_matches = sorted(allGood_matches, key=lambda x: x.distance)  # Sort them in the order of their distance
    best_matches = allGood_matches[:10]     # Append the 10 best matches from allGood_matches in best_matches

    print(len(allGood_matches))
    #  If allGood_matches has enough good matches then proceed
    if len(allGood_matches) > MIN_MATCH_COUNT:
        src_pts = np.float32([kpCam[m.queryIdx].pt for m in best_matches]).reshape(-1, 1, 2)  # Get the points of the best_matches
        points = [p[0] for p in src_pts]
        center = calculateCenter(points)   # Calculate the center of these points
        avgDistance = calculateDistance(src_pts, center)    # Calculate the average distance between the points and the center
        drawPoints = []

        # If the distance to the center is greater than the average distance then don't use it
        for p in src_pts:
            if ((abs(p[0][0]) - center[0] + abs(p[0][1]) - center[1]) / 2) < ((avgDistance[0] + avgDistance[1]) / 2):
                drawPoints.append(p)

        rectanglePts = findRectanglePoints(drawPoints)  # Find the right points for a rectangle based on the drawPoints

        # If rectanglePts is not None then draw a rectangle on the frame
        if rectanglePts[0] is not None or rectanglePts[1] is not None:
            return rectanglePts

    else:
        return False
