import pyzbar.pyzbar as pyzbar


class QRReader:
    qrObject = None

    def __int__(self, cap):
        self.cap = cap

    def getFrame(self):
        _, frame = self.cap.read()
        decodedObjects = pyzbar.decode(frame)
        return decodedObjects

    def findQR(self, searchQR):
        for obj in self.getFrame():
            qrdata = obj.data
            if qrdata == searchQR:
                self.qrObject = obj
                return obj
            else:
                return False

    def getWidthQR(self):
        x, y, w, h = self.qrObject.rect
        return w