import sys
sys.path.append('Imp_Functions/')

import cv2
from qrReader import QRReader
from helpFunctions import *

def startQRDistance(findQRCode):
    cap = cv2.VideoCapture(0)
    qrReader = QRReader(cap)

    qr = qrReader.findQR(findQRCode)

    if qr is not False:
        distance = calculateDistance(qrReader.getWidthQR(), 15, 1000)
        return distance
    else:
        return False
