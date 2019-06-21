import pyzbar.pyzbar as pyzbar


class QRReader:
    qrObject = None

    def __init__(self, frame):
        self.frame = frame

    def getFrame(self):
        decodedObjects = pyzbar.decode(self.frame)
        return decodedObjects

    def findQR(self, searchQR):
        for obj in self.getFrame():
            qrdata = obj.data.decode("utf-8")
            if qrdata == searchQR:
                self.qrObject = obj
                return obj
            else:
                return None

    def getRectangleQR(self):
        x, y, w, h = self.qrObject.rect
        return x, y, w, h

