import sys

sys.path.append('Imp_Functions/')
sys.path.append('../pi/')

import cv2
import numpy as np
from helpFunctions import *

cap = cv2.VideoCapture(0)
ESC = 27

def viewBeam():
    while True:
        _, frame = cap.read()

        x = 100
        y = 100
        w = 250
        h = 400

        cv2.circle(frame, ((x + w) / 2, (y + h) / 2), 20, (0, 255, 0), -1)

        cv2.imshow('Frame', frame)
        key = cv2.waitKey(20)
        if key == ESC:
            break

    cv2.destroyAllWindows()
    cap.release()

viewBeam()