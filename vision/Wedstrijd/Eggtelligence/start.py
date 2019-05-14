import numpy as np
import cv2
import pyzbar.pyzbar as pyzbar
from qrReader import main as ShowQR
from distanceToQR import main as GetDistance

cam = cv2.VideoCapture(0)

findQRCode = ["'s-Hertogenbosch"]


def main(findQRCode):
    _, frame = cam.read()

    qr = ShowQR(findQRCode)
    print("Found QR: " + qr.data)

    distance = GetDistance(qr)
    print("Found Distance: " + str(distance))

    return distance
    cam.release()


main(findQRCode)





