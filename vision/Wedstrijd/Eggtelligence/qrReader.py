import numpy as np
import cv2
import pyzbar.pyzbar as pyzbar


class QRReader:
    qrObject = None
    cam = cv2.VideoCapture(0)
    _, frame = cam.read()
    decodedObjects = None

    def __int__(self):
        self.decodedObjects = pyzbar.decode(self.frame)

    def findQR(self, searchQR):
        for obj in self.decodedObjects:
            qrdata = obj.data
            if qrdata == searchQR:
                self.qrObject = obj
                return obj
            else:
                return None

    def getWidthQR(self):
        x, y, w, h = self.qrObject.rect
        return w