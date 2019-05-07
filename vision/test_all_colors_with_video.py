import cv2
import numpy as np
import os, sys
import math

#cam = cv2.VideoCapture(0)
cam = cv2.VideoCapture('bakjegroen.avi')

while(cam.isOpened()):

    #Add color to mask, blue, green, red, grey 
    boundaries = [
                #blue
                #([100,150,0], [140,255,255]),
                #green
                #([0,100,50],[100,255,255]),
                #([20,100,50],[100,255,255]),
                #([99,227,117],[119,227,117]),
                #([147,165,17],[191,128,148]),
                #([93,165,17],[191,128,148]),
                ([120,100,5],[191,110,158]),
                #([100,150,0], [140,255,255]),
                #([93,65,7],[191,50,58]),
                #rgb 6,17,11 = 147,64.7,6.7 = 147,165,17 # 74,134,148 = 191,50,58 = 191,128,148
                #bgr 11,17,6 = 93,64.7,6.7 #148,134,74 = 49,50,58
                #0,5,0 = 120,100,2 = 120,255,5 #90,146,158 = 191,43,62 = 191,110,158
                #red
                #([0,206,145],[12,226,225]), 
                #([0,157,54],[189,250,255]),
                #([0,181,99],[100,238,240]),
                #([0,157,54],[100,238,240]),
                #([0,140,44],[100,238,240]),
                #grey
                #([10,200,0],[180,20,255])
                #([50,200,50],[150,20,255])
             ]

    # loop over the boundaries, yellow
    lower_yellow = np.array([20, 100, 100])
    upper_yellow = np.array([30, 255, 255])

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
            if(area > 1600 and factor >0 and factor <0.88):
                frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 255), 2)
        cv2.imshow('images', frame)

        k = cv2.waitKey(1) & 0xFF
        #escape key
        if k == 27:
            break

cam.release()
cv2.destroyAllWindows()
