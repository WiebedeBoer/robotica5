import cv2
import numpy as np
import os, sys
import math

def BlackTape():
    cam = cv2.VideoCapture(0)
    #cam = cv2.VideoCapture('tape.avi')    

    while(cam.isOpened()):
        #black tape
        lower_black = np.array([30,227,18])
        upper_black = np.array([105,94,84])
        #[30,227,18],[105,94,84]

        while (True):
            _, frame = cam.read()
            #hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
            hsv = cv2.cvtColor(frame, cv2.COLOR_RGB2HSV)
            mask = cv2.inRange(hsv, lower_black, upper_black)

            output = cv2.bitwise_and(frame, frame, mask = mask)
            contours, hierarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

            for cnr in range(len(contours)):
                cnt = contours[cnr]
                area = cv2.contourArea(cnt)
                perimeter = cv2.arcLength(cnt, True)
                if(perimeter >0):
                    factor = 4 * math.pi * area / perimeter**2
                else:
                    factor = 0
                x, y, w, h = cv2.boundingRect(cnt)
                if(area > 100 and factor >0 and factor <0.88):
                    frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)
            cv2.imshow('images', frame)

            k = cv2.waitKey(1) & 0xFF
            #escape key
            if k == 27:
                break

    cam.release()
    cv2.destroyAllWindows()
    
BlackTape()
