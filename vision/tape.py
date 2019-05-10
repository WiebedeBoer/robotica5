import cv2
import numpy as np
import os, sys
import math

def BlackTape():

    cap = cv2.VideoCapture(0)
    #cap = cv2.VideoCapture('tape.avi')
     
    while(True):
        #Capture frame-by-frame
        ret, frame = cap.read() 
        # Our operations on the frame come here
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        #mask
        ret, th = cv2.threshold(gray,10,255,cv2.THRESH_BINARY_INV)
        #mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
        #contours
        contours, hierarchy = cv2.findContours(th, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        for cnr in range(len(contours)):
            cnt = contours[cnr]
            area = cv2.contourArea(cnt)
            perimeter = cv2.arcLength(cnt, True)
            #factor calculation
            if(perimeter >0):
                factor = 4 * math.pi * area / perimeter**2
            else:
                factor = 0
            x, y, w, h = cv2.boundingRect(cnt)
            #area and shape
            if(area >200 and factor >0 and factor <0.88):
                gray = cv2.rectangle(gray, (x, y), (x + w, y + h), (0, 0, 0), 2)
        # Display the resulting frame
        cv2.imshow('frame',gray)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    # When everything done, release the capture
    cap.release()
    cv2.destroyAllWindows()

BlackTape()
    
#   frame = cv2.VideoCapture('tape.avi')

#    while(frame.isOpened()):
#        _, frame = frame.read()
#        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
#        blur = cv2.GaussianBlur(gray,(5,5),0)
#        ret, th = cv2.threshold(blur,10,255,cv2.THRESH_BINARY_INV)

#        contours, hierarchy = cv2.findContours(th,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
#        for cnt in contours:
#            area = cv2.contourArea(cnt)
#            if area > 100:
            #yellow contour
#                cv2.drawContours(frame, [cnt], -1, (0,255,255), 3)
                #frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)
                
#        cv2.imshow('frame',frame)
#        k = cv2.waitKey(1) & 0xFF
        #escape key
#        if k == 27:
#            break
#        output = cv2.bitwise_and(frame, frame, mask = mask)
#       mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
#        contours, hierarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
#        for cnr in range(len(contours)):
#           cnt = contours[cnr]
#            area = cv2.contourArea(cnt)
#            perimeter = cv2.arcLength(cnt, True)
#           if(perimeter >0):
#               factor = 4 * math.pi * area / perimeter**2
#
#
#
    

