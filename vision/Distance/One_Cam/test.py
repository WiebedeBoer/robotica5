
import cv2
import numpy as np

cam = cv2.VideoCapture(0)
lower_blue = np.array([110,50,50])
upper_blue = np.array([130,255,255])

while (True):
    ret, frame = cam.read()

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, lower_blue, upper_blue)
    res = cv2.bitwise_and(frame,frame, mask= mask)

    cv2.imshow('camera', res)
    k = cv2.waitKey(1) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()