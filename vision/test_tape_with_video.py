import cv2
import numpy as np
import os, sys
import math

#cam = cv2.VideoCapture(0)
cam = cv2.VideoCapture('tape.avi')

while(cam.isOpened()):

    #Add color to mask, blue, green, red, grey 
    boundaries = [
                #black tape
                ([30,227,18],[105,94,84]),
                #18,10,2 = 30,89,7 = 30,227,18 #62,85,54 = 105,37,33 = 105,94,84
                #black tape 2
                ([0,0,0],[11,36,79])
                # = = #79,70,68 = 11,14,31 = 11,36,79
             ]

    # loop over the boundaries, yellow bakje
    lower_yellow = np.array([20,100,100])
    upper_yellow = np.array([30,255,255])

    while (True):
        _, frame = cam.read()
        #hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        hsv = cv2.cvtColor(frame, cv2.COLOR_RGB2HSV)
        mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
    
        for (lower, upper) in boundaries:
            lower = np.array(lower, dtype = "uint8")
            upper = np.array(upper, dtype = "uint8")

            #find the colors within the specified boundaries and apply the mask
            in_mask = cv2.inRange(hsv, lower, upper)
            mask = mask + in_mask
        output = cv2.bitwise_and(frame, frame, mask = mask)
        contours, hierarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        #for cnt in contours:
        #for cnt in range(len(contours)):
        for cnr in range(len(contours)):
            cnt = contours[cnr]
            area = cv2.contourArea(cnt)
            perimeter = cv2.arcLength(cnt, True)
            if(perimeter >0):
                factor = 4 * math.pi * area / perimeter**2
            else:
                factor = 0
            x, y, w, h = cv2.boundingRect(cnt)
            #if(area > 1600):
            if(area > 200 and factor >0 and factor <0.88):
                frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 0), 2)

        cv2.imshow('images', frame)

        k = cv2.waitKey(1) & 0xFF
        #escape key
        if k == 27:
            break

cam.release()
cv2.destroyAllWindows()
