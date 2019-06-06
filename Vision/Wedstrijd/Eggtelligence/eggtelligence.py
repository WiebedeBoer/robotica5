import sys
sys.path.append('Imp_Functions/')
sys.path.append('Wedstrijd/Eggtelligence/')

from fm_ORB import fm_ORB
import cv2
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
    return func(frame, argument1)


def eggDistance(frame, argument):
    closestDistance = None
    detectEgg = DetectEgg(frame, None)
    eggDetected = detectEgg.DetectEgg()
    allDistances = []

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


def qrDistance(frame, argument):
    findQRCode = townSwitcher(argument)
    qrReader = QRReader(frame)
    qr = qrReader.findQR(findQRCode)

    if qr is not None:
        return calculateDistance(qrReader.getWidthQR(), 15, 1000)
    else:
        return False

def chicken(frame):
    imgTrainColor = cv2.imread('Images/TrainImg/Kip/kip1.jpg')
    return fm_ORB(frame, imgTrainColor)


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


