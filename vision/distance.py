import cv2
import numpy as np
import os, sys
import math
from decimal import *
import glob

cam = cv2.VideoCapture('bakjegrijs.h264')

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
              #[([20, 100, 100], [30, 255, 255])], #Yellow
              #[([100,150,0], [140,255,255])], #Blue
              #[([20, 100, 100], [30, 255, 255])], #Red
              [
               #([90, 7, 50], [125, 40, 192]),
               #([97, 30, 37],[123, 135, 146]), #Pakt grijs bakje redelijk, pakt nog net teveel
               #([105, 16, 19],[125, 36, 99]), #Pakt grijs bakje redelijk
               #([116, 9, 26],[136, 29, 127]), #LightGray
               #([90, 8, 20],[135, 130, 130]), #Vrijwel Perfect?
               #([87, 10, 24],[130, 31, 125]),
               #([116, 9, 28],[136, 29, 108]),
               #([85, 5, 15],[125, 130, 130]),
               #([93, 74, 69],[113, 94, 149]),
               #([4, 15, 74],[24, 35, 154]),
               ##([96, 94, 19],[116, 114, 99]),
               #([90, 24, 94],[110,  44, 174]), #MiddleGray
               #([116, 5, 47],[136,25,127]), #Doet het al redelijk
               ##([116, 34, 32],[114, 42, 163]),
               ##([4, 12,5],[160,63,255]) #Pakt teveel
              ] #Gray
              #[#([79, 164, 23],[33, 24, 115]),
               #([62, 245, 0],[115, 80, 45]),
               #([7, 85, 30],[70, 13, 119]),
               #([79, 164, 23],[33, 24, 115])
              #  ([0,0,0],[10,10,40]), ###
                #([82, 245, 1],[104, 265, 81])
                #([79, 167, 32],[99, 187, 112]), ###
                #([80, 101, 80],[100, 121, 161]) #Werkt aardig toed op de huidige film
                #([31, 147, 0],[51, 167, 53])
                
              #]#Groen
         ]

totalContours = []


def main():
    while (1):
        _, frame = cam.read()
        img = cv2.GaussianBlur(frame, (15, 15), 0)
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        for range in boundaries:
            in_mask = None;
            for (lower, upper) in range:
                # Find the colors within the specified boundaries and apply the mask
                if(in_mask is None):
                    in_mask = cv2.inRange(hsv, np.array(lower, dtype = "uint8"), np.array(upper, dtype = "uint8"));
                else:
                    in_mask = in_mask + cv2.inRange(hsv, np.array(lower, dtype = "uint8"), np.array(upper, dtype = "uint8"))
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
