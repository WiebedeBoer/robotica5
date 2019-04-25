import cv2
import numpy as np
import os, sys
import math

cam = cv2.VideoCapture(0)

font                   = cv2.FONT_HERSHEY_SIMPLEX
bottomLeftCornerOfText = (10,500)
fontScale              = 1
fontColor              = (255,255,255)
lineType               = 2

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
    contours, hierarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    for cnt in contours:
        area = cv2.contourArea(cnt)
        perimeter = cv2.arcLength(cnt, True)
        x, y, w, h = cv2.boundingRect(cnt)
        if(area > 1600):
            frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 255), 2)
            
            cv2.putText(frame,str(str(w) + " " + str(h)),
                        bottomLeftCornerOfText,
                        font,
                        fontScale,
                        fontColor,
                        lineType)


    cv2.imshow('images', frame)

    k = cv2.waitKey(1) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()
