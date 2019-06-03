import numpy as np
import cv2
import time


def sortSecond(val):
    return val[1]


ESC = 27

width = 640
height = 480

cap = cv2.VideoCapture(0)
cap.set(3, 1280)
cap.set(4, 720)

orb = cv2.ORB_create()
bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)

imgTrainColor = cv2.imread('../img/png/original_kip.jpeg')
imgTrainGray = cv2.cvtColor(imgTrainColor, cv2.COLOR_BGR2GRAY)

kpTrain = orb.detect(imgTrainGray, None)
kpTrain, desTrain = orb.compute(imgTrainGray, kpTrain)

firsttime = True
MIN_MATCH_COUNT = 50

while True:
    ret, imgCamColor = cap.read()
    imgCamGray = cv2.cvtColor(imgCamColor, cv2.COLOR_BGR2GRAY)
    kpCam = orb.detect(imgCamGray, None)
    kpCam, desCam = orb.compute(imgCamGray, kpCam)
    bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)
    matches = bf.match(desCam, desTrain)

    allGood_matches = []
    best_matches = []

    if firsttime is True:
        h1, w1 = imgCamColor.shape[:2]
        h2, w2 = imgTrainColor.shape[:2]
        nWidth = w1 + w2
        nHeight = max(h1, h2)
        hdif = (h1 - h2) / 2
        firsttime = False

    for m in matches:
        if (m.distance / 100) < 0.5:
            allGood_matches.append(m)

    # Sort them in the order of their distance.
    allGood_matches = sorted(allGood_matches, key=lambda x: x.distance)
    best_matches = allGood_matches[:10]

    if len(allGood_matches) > MIN_MATCH_COUNT or len(best_matches) is 9:
        print("Enough matches are found - %d/%d" % (len(allGood_matches), MIN_MATCH_COUNT))

        src_pts = np.float32([kpCam[m.queryIdx].pt for m in best_matches]).reshape(-1, 1, 2)
        dst_pts = np.float32([kpTrain[m.trainIdx].pt for m in best_matches]).reshape(-1, 1, 2)
        M, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 1.0)
        matchesMask = mask.ravel().tolist()

        pts = np.float32([[0, 0], [0, h1 - 1], [w1 - 1, h1 - 1], [w1 - 1, 0]]).reshape(-1, 1, 2)
        dst = cv2.perspectiveTransform(pts, M)
        dst += (w1, 0)  # adding offset

        imgCamColor = cv2.polylines(imgCamColor, [np.int32(dst)], True, (0, 0, 255), 3, cv2.LINE_AA)

    else:
        print("Not enough matches are found - %d/%d" % (len(allGood_matches), MIN_MATCH_COUNT))

    result = np.zeros((nHeight, nWidth, 3), np.uint8)
    result[hdif:hdif + h2, :w2] = imgTrainColor
    result[:h1, w2:w1 + w2] = imgCamColor

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
