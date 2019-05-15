import sys
sys.path.append('../../Imp_Functions/')

from qrReader import QRReader
from distance import Distance


findQRCode = "http://'s-Hertogenbosch"


def eggTelligence(findQRCode):
    qrReader = QRReader()
    getDistance = Distance(15, 1000)

    while True:
        qr = qrReader.findQR(findQRCode)

        if qr is not None:
            print("Found QR: " + qr.data)
            distance = getDistance.calibration(qrReader.getWidthQR())
            print("Found Distance: " + str(distance))


eggTelligence(findQRCode)





