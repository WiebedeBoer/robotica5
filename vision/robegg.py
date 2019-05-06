import numpy as np
import cv2
import math
import random

def nothing():
    pass

def ShowCamFeed():
    cap = cv2.VideoCapture(0)

    while(True):
        _, frame = cap.read()
        #hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        
        #cv2.imshow('frame', frame)

        #define range of color
        lower = np.array([165,150,125])
        upper = np.array([255,255,255])
        mask = cv2.inRange(frame, lower, upper)
        #cv2.imshow("inRange", mask)
        res = cv2.bitwise_and(frame,frame, mask= mask)
        #cv2.imshow("Frame + Mask", res)

        cv2.imshow("B4 gray", mask)
        hsv = cv2.cvtColor(res, cv2.COLOR_BGR2GRAY)

        cv2.imshow("test", hsv)

        #th3 = cv2.adaptiveThreshold(hsv,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,11,2)

        ret, th = cv2.threshold(hsv,200,255,cv2.THRESH_BINARY_INV)
        contours, hierarchy = cv2.findContours(th,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

        for cnt in contours:
            area = cv2.contourArea(cnt)
            perimeter = cv2.arcLength(cnt,True)
            if(perimeter > 0):
                vormfactor = 4 * math.pi * area / perimeter ** 2
                if area > 200:
                    cv2.drawContours(frame, cnt, -1, (0,255,0), 3)
                    if vormfactor > 0.7:
                        x,y,w,h = cv2.boundingRect(cnt)
                        frame = cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,255),2)

        cv2.drawContours(frame, contours, -1, (0,255,0), 3)

        cv2.imshow('default + contours', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    cap.release()
    cv2.destroyAllWindows()

ShowCamFeed()
