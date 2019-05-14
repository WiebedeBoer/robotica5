import numpy as np
import cv2
from qrReader import ShowQR

def main():
    cam = cv2.VideoCapture(0)
    _, frame = cam.read()
    allQRCodes = []
    while True:
        if ShowQR() != None:
            allQRCodes.append(ShowQR())
            print("Dikke boktor")

        if len(allQRCodes) != 0:
            for qr in allQRCodes:
                text = None

        # Check if "esc" is pressed
        if (cv2.waitKey(1) & 0xFF) == 27:
            break

    cam.release()
    cv2.destroyAllWindows()


main()





