import sys

sys.path.append('Imp_Functions/')

import cv2
from eggDetection import DetectEgg
from helpFunctions import *


def eggDistance(frame):

    closestDistance = None
    allDistances = []
    detectEgg = DetectEgg(frame, None)
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
        return closestDistance
    else:
        return False
