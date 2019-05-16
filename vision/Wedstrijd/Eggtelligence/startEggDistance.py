import sys
sys.path.append('../../Imp_Functions/')

from eggDetection import DetectEgg
from distance import Distance

def eggTelligence():
    detectEgg = DetectEgg(False)
    getDistance = Distance(15, 1000)

    while True:
         = 

        if qr is not None:
            print("Found QR: " + qr.data)
            distance = getDistance.calibration(qrReader.getWidthQR())
            print("Found Distance: " + str(distance))


eggTelligence(findQRCode)





