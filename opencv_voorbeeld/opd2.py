import cv2
import numpy as np
import math

def testblue():
    frame = cv2.imread('C:/Users/wiebe/Documents/robotica/opencv_workshop_materiaal(4)/opencv-logo-white.png')
    #hsv
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower_red = np.array([0,50,50])
    upper_red = np.array([10,255,255])
    redmask = cv2.inRange(hsv, lower_red, upper_red)
    #contours
    #imgray = cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)
    #ret,thresh = cv2.threshold(imgray,127,255,0)
    #im2, contours, hierarchy = cv2.findContours(redmask,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    contours, hierarchy = cv2.findContours(redmask,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    #contours
    
    cnt = contours[0]
    
    M = cv2.moments(cnt)
    cnt = cv2.convexHull(cnt)
    cy = int(M['m01']/M['m00'])
    cx = int(M['m10']/M['m00'])
    #area = cv2.contourArea(cnt)
    #perimeter = cv2.arcLength(cnt,True)
    #draw contours
    cv2.drawContours(frame, [cnt], -1, (0,255,0), 3)
    #draw a circle
    cv2.circle(frame,(cx,cy), 5, (0,255,255), -1)
    

    
    #define range of color blue
    #lower_blue = np.array([110,50,50])
    #upper_blue = np.array([130,255,255])
    #define range of color red
    #lower_red = np.array([0,50,50])
    #upper_red = np.array([10,255,255])
    # Threshold the HSV image to get only blue colors
    #mask = cv2.inRange(hsv, lower_blue, upper_blue)
    #green mask
    #greenmask = cv2.inRange(hsv, lower_green, upper_green)
    #red mask
    #redmask = cv2.inRange(hsv, lower_red, upper_red)
    # Bitwise-AND mask and original image
    #res = cv2.bitwise_and(frame,frame, mask= mask)
    #green res
    #greenres = cv2.bitwise_and(frame,frame, mask= greenmask)
    #red res
    #redres = cv2.bitwise_and(frame,frame, mask= redmask)

    #original image
    cv2.imshow('frame',frame)
    #blue image
    #cv2.imshow('res',res)
    #print k
    k = cv2.waitKey(0)
    print k
    #destroy
    cv2.destroyAllWindows()


