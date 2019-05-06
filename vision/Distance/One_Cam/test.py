import cv2
import numpy as np
import os, sys

cam = cv2.VideoCapture(0)

#Add color blue to mask
boundaries = [
            ([100,150,0], [140,255,255])
         ]

# loop over the boundaries
lower_yellow = np.array([20, 100, 100])
upper_yellow = np.array([30, 255, 255])

while (True):
    _, frame = cam.read()
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
    
    for (lower, upper) in boundaries:
        lower = np.array(lower, dtype = "uint8")
        upper = np.array(upper, dtype = "uint8")

        #find the colors within the specified boundaries and apply the mask
        in_mask = cv2.inRange(hsv, lower, upper)
        mask = mask + in_mask

    output = cv2.bitwise_and(frame, frame, mask = mask)
    cv2.imshow('images', output)

    k = cv2.waitKey(1) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()
