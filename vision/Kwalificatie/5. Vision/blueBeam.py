import cv2
import numpy as np
import os, sys
import math
from decimal import *
import glob

def ViewBeam():
    # Text settings
    font = cv2.FONT_HERSHEY_SIMPLEX
    bottomLeftCornerOfText = (10,500)
    fontScale = 0.75
    fontColor = (255,255,255)
    lineType = 2
    #video capture
    cap = cv2.VideoCapture(0)
    xc, yc = 0, 0
    while(1):
        #wait for socket
        #sok.recv(1024,0)
        _, frame = cap.read()
        #25,87,154 = 211,84,60 = 211,214,153 #27,94,164 = 211,84,64 = 211,214,163
        #([100,150,0], [140,255,255])
        # go over the boundaries, blue beam
        #([0,140,44],[100,238,240])
        #lower_blue = np.array([211,214,153])
        #upper_blue = np.array([211,214,163])
        lower_blue = np.array([0,140,44])
        upper_blue = np.array([100,238,240])
        #mask
        hsv = cv2.cvtColor(frame, cv2.COLOR_RGB2HSV)
        mask = cv2.inRange(hsv, lower_blue, upper_blue)
        #contours
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
            #screen coordinates
            res = cv2.bitwise_and(frame,frame,mask=mask)
            keypoints = (res)
            if len(keypoints) == 1:
                xc = int(keypoints[0].pt[0])
                yc = int(keypoints[0].pt[1])
            if(area >100 and area <3600 and factor >0 and factor <0.99):
                #set contour around object
                frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 0), 2)
                #calculate distance
                distance = calculateDistance(w)
                #print command
                print("FollowBeam;"+str(w)+";"+str(h)+";"+str(xc)+";"+str(yc)+"\n")
            elif(area >3600 and factor >0 and factor <0.99):
                #set contour around object
                frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)
                #calculate distance
                distance = calculateDistance(w)
                #apply text
                cv2.putText(frame,str(str(w)+ " " + str(h) + " " + str(round(distance, 2))),(x + (w / 2), y),font,fontScale,fontColor,lineType)
                #print command
                print("StopBeam;\n")
            #else:
                #print command
                print("SearchBeam;\n")
        #show vid
        cv2.imshow("Frame", frame)
        #key
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    cap.release()
    cv2.destroyAllWindows()

#calibrate distance
def calibration(w):
    # Initialize the known distance from the camera to the object
    KNOWN_DISTANCE = 100.0
    # Initialize the known object width, which in this case
    KNOWN_WIDTH = Decimal(15.0)
    # Initialize the known object focal length, which in this case
    FOCAL_LENGTH = 1013
    #return calculation
    return (KNOWN_DISTANCE * (w / 2)) / (KNOWN_WIDTH / 2)

#calculate distance
def calculateDistance(w):
    # Initialize the known distance from the camera to the object
    KNOWN_DISTANCE = 100.0
    # Initialize the known object width, which in this case
    KNOWN_WIDTH = Decimal(15.0)
    # Initialize the known object focal length, which in this case
    FOCAL_LENGTH = 1013
    #return calculation
    return (FOCAL_LENGTH * (KNOWN_WIDTH / 2)) / (w / 2)

#beam function
ViewBeam()
