import sys
sys.path.append('../../Imp_Functions/')

import cv2
from qrReader import QRReader
from helpFunctions import *


findQRCode = "http://'s-Hertogenbosch"


def startQRDistance(findQRCode):
    cap = cv2.VideoCapture(0)
    qrReader = QRReader(cap)

    while True:
        qr = qrReader.findQR(findQRCode)

        if qr is not False:
            print("Found QR: " + qr.data)
            distance = calculateDistance(qrReader.getWidthQR(), 15, 1000)
            print("Found Distance: " + str(distance))


startQRDistance(findQRCode)





