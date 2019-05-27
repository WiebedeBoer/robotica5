import sys
sys.path.append('Imp_Functions/')
sys.path.append('Wedstrijd/Eggtelligence/')

import cv2
from helpFunctions import *
from qrReader import QRReader

def startQRDistance(findQRCode, frame):
    qrReader = QRReader(frame)

    qr = qrReader.findQR(findQRCode)

    if qr is not None:
        distance = calculateDistance(qrReader.getWidthQR(), 15, 1000)
        return distance
    else:
        return False
