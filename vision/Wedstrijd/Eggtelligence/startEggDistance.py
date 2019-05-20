import sys
sys.path.append('Imp_Functions/')
sys.path.append('../pi/')

import cv2
from eggDetection import DetectEgg
from helpFunctions import *
from camera_opencv import getCapture


def startEggDistance():

    cap = getCapture()

    closestDistance = None
    allDistances = []
    detectEgg = DetectEgg(False, cap)
    eggDetected = detectEgg.DetectEgg()

    if eggDetected is not False:
        allKeypointsWidths = detectEgg.widthKeypoints(eggDetected)

        for width in allKeypointsWidths:
            distance = calculateDistance(width, 5.4, 395.684786196)
            allDistances.append(distance)

    for distance in allDistances:
        if closestDistance is None:
            closestDistance = distance
        elif distance < closestDistance:
            closestDistance = distance

    if closestDistance is not None:
        print("Closest distance found")
        return closestDistance
    else:
        print("Not a distance found")
        return 0