import cv2
import numpy as np
import math

def testblue():
    frame = cv2.imread('C:/Users/wiebe/Documents/robotica/opencv_workshop_materiaal(4)/opencv-logo-white.png')
    #hsv
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    #define range of color blue
    lower_blue = np.array([110,50,50])
    upper_blue = np.array([130,255,255])
    #define range of color green
    lower_green = np.array([50,50,50])
    upper_green = np.array([70,255,255])
    #define range of color red
    lower_red = np.array([0,50,50])
    upper_red = np.array([10,255,255])
    #define range of color red
    # Threshold the HSV image to get only blue colors
    mask = cv2.inRange(hsv, lower_blue, upper_blue)
    #green mask
    greenmask = cv2.inRange(hsv, lower_green, upper_green)
    #red mask
    redmask = cv2.inRange(hsv, lower_red, upper_red)
    # Bitwise-AND mask and original image
    res = cv2.bitwise_and(frame,frame, mask= mask)
    #green res
    greenres = cv2.bitwise_and(frame,frame, mask= greenmask)
    #red res
    redres = cv2.bitwise_and(frame,frame, mask= redmask)
    #original image
    cv2.imshow('frame',frame)
    #blue image
    cv2.imshow('res',res)
    #green image
    cv2.imshow('greenres',greenres)
    #red image
    cv2.imshow('redres',redres)
    #print k
    k = cv2.waitKey(0)
    print k
    #destroy
    cv2.destroyAllWindows()


