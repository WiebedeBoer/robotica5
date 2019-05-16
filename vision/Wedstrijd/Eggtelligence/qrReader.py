import cv2
import pyzbar.pyzbar as pyzbar


class QRReader:
    qrObject = None
    cam = cv2.VideoCapture(0)

    def getFrame(self):
        _, frame = self.cam.read()
        decodedObjects = pyzbar.decode(frame)
        return decodedObjects

    def findQR(self, searchQR):
        for obj in self.getFrame():
            qrdata = obj.data
            if qrdata == searchQR:
                self.qrObject = obj
                return obj
            else:
                return None

    def getWidthQR(self):
        x, y, w, h = self.qrObject.rect
        return w