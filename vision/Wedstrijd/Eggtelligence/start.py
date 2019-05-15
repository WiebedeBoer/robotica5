import cv2
from qrReader import QRReader
from distance import Distance

findQRCode = "http://'s-Hertogenbosch"




def eggTelligence(findQRCode):
    qrReader = QRReader()
    getDistance = Distance(15, 1000, 100)

    while True:
        qr = qrReader.findQR(findQRCode)

        if qr is not None:
            print("Found QR: " + qr.data)
            distance = getDistance.calculateDistance(qrReader.getWidthQR())
            print("Found Distance: " + str(distance))

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break


eggTelligence(findQRCode)





