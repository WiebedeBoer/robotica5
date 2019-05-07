import cv2
import numpy as np
import os, sys
import math
from decimal import *

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
              #([100,150,0], [140,255,255])
         ]

# Loop over the boundaries
lower_yellow = np.array([20, 100, 100])
upper_yellow = np.array([30, 255, 255])

def main():
    while (True):
        _, frame = cam.read()
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
        
        for (lower, upper) in boundaries:
            lower = np.array(lower, dtype = "uint8")
            upper = np.array(upper, dtype = "uint8")

            # Find the colors within the specified boundaries and apply the mask
            in_mask = cv2.inRange(hsv, lower, upper)
            mask = mask + in_mask
    
        output = cv2.bitwise_and(frame, frame, mask = mask)
        contours, hierarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        for cnt in contours:
            area = cv2.contourArea(cnt)
            perimeter = cv2.arcLength(cnt, True)
            x, y, w, h = cv2.boundingRect(cnt)
            if(area > 1600):
                # Set contour arround object
                frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 255), 2)
                
                # Distance to object calculating
                distance = calculateDistance(w)
                
                # Calculating per cm how many pixels there are - 0.048 / 50 = (3 / 3125)
                # RelLength = Decimal(3.0 / 3125.0) * Decimal(distance)
                
                # Calculating object pixels width at 0 cm
                pxWidthObjAtZeroCM = w * Decimal(Decimal(1.2013) ** Decimal(distance / 10))
                
                # Calculating object pixels width at calculated distance
                pxWidthObjAtDistanceCM = pxWidthObjAtZeroCM * Decimal(Decimal(0.833) ** Decimal(distance / 10))

                # KNOWN_WIDTH =  (768 - w) * relLength

                # Calibration
                #focalLength = calibration(w)


                # Apply text in frame
                cv2.putText(frame,str(str(w)+ " " + str(round(pxWidthObjAtDistanceCM, 2)) + " " + " " + str(h) + " " + str(round(distance, 2))),
                            (x + (w / 2), y),
                            font,
                            fontScale,
                            fontColor,
                            lineType)
                
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
