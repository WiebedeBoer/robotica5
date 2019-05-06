import cv2
import numpy as np
import os, sys
import math
from decimal import *
import glob

cam = cv2.VideoCapture(0)

# Distance calculating
    # Initialize the known distance from the camera to the object
KNOWN_DISTANCE = 100.0
    # Initialize the known object width, which in this case
KNOWN_WIDTH = Decimal(15.0)
  # Initialize the known object focal length, which in this case
FOCAL_LENGTH = 1013

# Text settings
font                   = cv2.FONT_HERSHEY_SIMPLEX
bottomLeftCornerOfText = (10,500)
fontScale              = 0.75
fontColor              = (255,255,255)
lineType               = 2

# Add color blue to mask
boundaries = [
              ([100,150,0], [140,255,255]),
              ([20, 100, 100], [30, 255, 255]),
         ]

totalContours = []

# Loop over the boundaries
lower_yellow = np.array([20, 100, 100])
upper_yellow = np.array([30, 255, 255])

def main():
    while (1):
        _, frame = cam.read()
        img = cv2.GaussianBlur(frame, (15, 15), 0)
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
        
        for (lower, upper) in boundaries:
            # Find the colors within the specified boundaries and apply the mask
            in_mask = cv2.inRange(hsv, np.array(lower, dtype = "uint8"), np.array(upper, dtype = "uint8"))
            _, res = cv2.threshold(in_mask, 215, 255, cv2.THRESH_BINARY)
            res = cv2.GaussianBlur(res, (15, 15), 0)
            contours, hierarchy = cv2.findContours(res, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
            totalContours.append(contours)
        
        for contours in totalContours:
            for cnt in contours:
                area = cv2.contourArea(cnt)
                perimeter = cv2.arcLength(cnt, True)
                approx = cv2.approxPolyDP(cnt, 0.04 * perimeter, True)
                x, y, w, h = cv2.boundingRect(cnt)
                
                if(w > 40 and h > 10 and len(approx) >= 4 and len(approx) <= 6):
                    # Set contour arround object
                    frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 255), 2)
                    
                    # Distance to object calculating
                    distance = calculateDistance(w)
                    
                    # Apply text in frame
                    cv2.putText(frame,str(str(w)+ " " + str(h) + " " + str(round(distance, 2))),
                                (x + (w / 2), y),font,fontScale,fontColor,lineType)
        del totalContours[:]
                
        #Show frame
        cv2.imshow('images', frame)
        
        #Check if "esc" is pressed
        k = cv2.waitKey(1) & 0xFF
        if k == 27:
            break
    
    cv2.destroyAllWindows()

def calibration(w):
    return (KNOWN_DISTANCE * (w / 2)) / (KNOWN_WIDTH / 2)

def calculateDistance(w):
    return (FOCAL_LENGTH * (KNOWN_WIDTH / 2)) / (w / 2)

main()


