import numpy as np
import cv2
import pyzbar.pyzbar as pyzbar

def ViewBeam():
    cap = cv2.VideoCapture(0)
    while(1):
        _, frame = cap.read()

        cv2.imshow("Frame", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    cap.release()
    cv2.destroyAllWindows()

ViewBeam()
