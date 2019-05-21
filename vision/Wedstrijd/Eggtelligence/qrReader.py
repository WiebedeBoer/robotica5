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
            qrdata = obj.data
            if qrdata == searchQR:
                self.qrObject = obj
                return obj
            else:
                return None

    def getWidthQR(self):
        x, y, w, h = self.qrObject.rect
        return w