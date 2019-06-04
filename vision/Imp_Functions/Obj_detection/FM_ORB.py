import numpy as np
import cv2


def calculateCenter(pts):
    allXPoints = []
    allYPoints = []
    sumX = 0
    sumY = 0

    for p in pts:
        allXPoints.append(p[0][0])
        allYPoints.append(p[0][1])

    for x in allXPoints:
        sumX += x

    for y in allYPoints:
        sumY += y

    return [sumX / len(pts), sumY / len(pts)]


def calculateDistance(pts, center):
    allXDistances = []
    allYDistances = []
    sumX = 0
    sumY = 0

    for p in pts:
        allXDistances.append((abs(p[0][0]) - center[0]))
        allYDistances.append((abs(p[0][1]) - center[0]))

    for x in allXDistances:
        sumX += x

    for y in allYDistances:
        sumY += y

    return [sumX / len(pts), sumY / len(pts)]


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


def sortSecond(val):
    return val[1]

def FM_ORB():
    ESC = 27

    cap = cv2.VideoCapture(0)
    cap.set(3, 720)
    cap.set(4, 720)

    # Create the orb instance
    orb = cv2.ORB_create()
    # Create a Brute Force Matcher with the algorithm NORM_HAMMING and crossCheck
    bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)

    imgTrainColor = cv2.imread('../img/png/original_kip.jpeg')
    imgTrainGray = cv2.cvtColor(imgTrainColor, cv2.COLOR_BGR2GRAY)

    kpTrain = orb.detect(imgTrainGray, None)   # Detect orbs in imgTrainGray
    kpTrain, desTrain = orb.compute(imgTrainGray, kpTrain)    # Detect descriptors in imgTrainGray

    firsttime = True
    MIN_MATCH_COUNT = 20

    while True:
        ret, imgCamColor = cap.read()
        imgCamGray = cv2.cvtColor(imgCamColor, cv2.COLOR_BGR2GRAY)

        kpCam = orb.detect(imgCamGray, None)    # Detect orbs in imgTrainGray
        kpCam, desCam = orb.compute(imgCamGray, kpCam)  # Detect descriptors in imgTrainGray
        matches = bf.match(desCam, desTrain)    # Check for matches between the two descriptors images

        allGood_matches = []    # If the mark is under a special ratio it is appended in this list
        best_matches = []   # Only an amount of the best good matches are appended in this list

        # Get shape and dimensions of the image and frame
        if firsttime is True:
            h1, w1 = imgCamColor.shape[:2]
            h2, w2 = imgTrainColor.shape[:2]
            nWidth = w1 + w2
            nHeight = max(h1, h2)
            hdif = (h1 - h2) / 2
            firsttime = False

        # For every match check if the match is good enough
        for m in matches:
            if (m.distance / 100) < 0.5:
                allGood_matches.append(m)

        allGood_matches = sorted(allGood_matches, key=lambda x: x.distance)  # Sort them in the order of their distance
        best_matches = allGood_matches[:10]     # Append the 10 best matches from allGood_matches in best_matches

        #  If allGood_matches has enough good matches then proceed
        if len(allGood_matches) > MIN_MATCH_COUNT:
            src_pts = np.float32([kpCam[m.queryIdx].pt for m in best_matches]).reshape(-1, 1, 2)  # Get the points of the best_matches
            center = calculateCenter(src_pts)   # Calculate the center of these points
            avgDistance = calculateDistance(src_pts, center)    # Calculate the average distance between the points and the center

            drawPoints = []

            # If the distance to the center is greater than the average distance then don't use it
            for p in src_pts:
                if ((abs(p[0][0]) - center[0] + abs(p[0][1]) - center[1]) / 2) < ((avgDistance[0] + avgDistance[1]) / 2):
                    drawPoints.append(p)

            rectanglePts = findRectanglePoints(drawPoints)  # Find the right points for a rectangle based on the drawPoints

            # If rectanglePts is not None then draw a rectangle on the frame
            if rectanglePts[0] is not None or rectanglePts[1] is not None:
                cv2.rectangle(imgCamColor, (rectanglePts[0][0], rectanglePts[0][1]),
                              (rectanglePts[1][0], rectanglePts[1][1]), (0, 255, 0), 3)

        result = np.zeros((nHeight, nWidth, 3), np.uint8)
        result[hdif:hdif + h2, :w2] = imgTrainColor
        result[:h1, w2:w1 + w2] = imgCamColor

        # Show the matches between the orbs in two images
        for i in range(len(best_matches)):
            pt_a = (int(kpTrain[best_matches[i].trainIdx].pt[0]), int(kpTrain[best_matches[i].trainIdx].pt[1] + hdif))
            pt_b = (int(kpCam[best_matches[i].queryIdx].pt[0] + w2), int(kpCam[best_matches[i].queryIdx].pt[1]))
            cv2.line(result, pt_a, pt_b, (255, 0, 0))

        cv2.imshow('Camera', result)

        key = cv2.waitKey(20)
        if key == ESC:
            break

    cv2.destroyAllWindows()
    cap.release()


FM_ORB()
