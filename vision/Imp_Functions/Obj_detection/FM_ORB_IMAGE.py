import numpy as np
import cv2
import glob


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


def start():
    imgDetectColor = cv2.imread('../img/testImg/test2.png')
    imgDetectColor = cv2.resize(imgDetectColor, (720, 720))
    imgDetectColorGray = cv2.cvtColor(imgDetectColor, cv2.COLOR_BGR2GRAY)

    imgTrainColor = cv2.imread('../img/png/original_kip.jpeg')
    imgTrainGray = cv2.cvtColor(imgTrainColor, cv2.COLOR_BGR2GRAY)

    orb = cv2.ORB_create()
    bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)

    kpTrain = orb.detect(imgTrainGray, None)
    kpTrain, desTrain = orb.compute(imgTrainGray, kpTrain)

    firstTime = True
    MIN_MATCH_COUNT = 0

    kpCam = orb.detect(imgDetectColorGray, None)
    kpCam, desCam = orb.compute(imgDetectColorGray, kpCam)
    matches = bf.match(desCam, desTrain)

    allGood_matches = []
    best_matches = []

    if firstTime is True:
        h1, w1 = imgDetectColor.shape[:2]
        h2, w2 = imgTrainColor.shape[:2]
        nWidth = w1 + w2
        nHeight = max(h1, h2)
        hdif = (h1 - h2) / 2
        firstTime = False

    for m in matches:
        if (m.distance / 100) < 0.6:
            allGood_matches.append(m)

    # Sort them in the order of their distance.
    allGood_matches = sorted(allGood_matches, key=lambda x: x.distance)
    best_matches = allGood_matches[:10]

    if len(allGood_matches) > MIN_MATCH_COUNT or len(best_matches) is 9:
        print("Enough matches are found - %d/%d" % (len(allGood_matches), MIN_MATCH_COUNT))

        src_pts = np.float32([kpCam[m.queryIdx].pt for m in best_matches]).reshape(-1, 1, 2)
        center = calculateCenter(src_pts)
        avgDistance = calculateDistance(src_pts, center)

        drawPoints = []
        for p in src_pts:
            if ((abs(p[0][0]) - center[0] + abs(p[0][1]) - center[1]) / 2) < ((avgDistance[0] + avgDistance[1]) / 2):
                drawPoints.append(p)

        rectanglePts = findRectanglePoints(drawPoints)
        cv2.rectangle(imgDetectColor, (rectanglePts[0][0], rectanglePts[0][1]),
                      (rectanglePts[1][0], rectanglePts[1][1]), (0, 255, 0), 3)

    else:
        print("Not enough matches are found - %d/%d" % (len(allGood_matches), MIN_MATCH_COUNT))

    result = np.zeros((nHeight, nWidth, 3), np.uint8)
    result[hdif:hdif + h2, :w2] = imgTrainColor
    result[:h1, w2:w1 + w2] = imgDetectColor

    for i in range(len(allGood_matches)):
        pt_a = (int(kpTrain[allGood_matches[i].trainIdx].pt[0]), int(kpTrain[allGood_matches[i].trainIdx].pt[1] + hdif))
        pt_b = (int(kpCam[allGood_matches[i].queryIdx].pt[0] + w2), int(kpCam[allGood_matches[i].queryIdx].pt[1]))
        cv2.line(result, pt_a, pt_b, (255, 0, 0))

    cv2.imshow('Camera', result)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

start()

