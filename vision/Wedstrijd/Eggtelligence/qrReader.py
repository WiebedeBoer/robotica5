import numpy as np
import cv2
import pyzbar.pyzbar as pyzbar

cam = cv2.VideoCapture(0)

def main(findQR):
    _, frame = cam.read()
    decodedObjects = pyzbar.decode(frame)

    for obj in decodedObjects:
        print("Data: ", obj.data, "\n")
        qrdata = obj.data

        if qrdata == findQR:
            return obj

    cam.release()


