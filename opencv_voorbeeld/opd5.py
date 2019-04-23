import cv2
import numpy as np
import math

def con():
    frame = cv2.imread('C:/Users/wiebe/Documents/robotica/opencv_workshop_materiaal(4)/dobbelstenen.png')
    #hsv
    #hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
    #blur = cv2.GaussianBlur(gray,(5,5),0)
    ret, th = cv2.threshold(gray,10,255,cv2.THRESH_BINARY)
    #lower_red = np.array([0,50,50])
    #upper_red = np.array([10,255,255])
    #redmask = cv2.inRange(hsv, lower_red, upper_red)
    #contours
    #imgray = cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)
    #ret,thresh = cv2.threshold(imgray,127,255,0)
    #im2, contours, hierarchy = cv2.findContours(redmask,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    #contours, hierarchy = cv2.findContours(redmask,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    contours, hierarchy = cv2.findContours(th,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    hierarchy = hierarchy[0]
    #print len(contours)
    dices =[]
    for cnr in range(len(contours)):
        #bound
        x,y,w,h = cv2.boundingRect(contours[cnr])
        #dice
        die = gray[y:y+h, x:x+w]

        #cv2.imshow('die'+str(cnr),die)

        ret, eye = cv2.threshold(die,200,255,cv2.THRESH_BINARY_INV)
        contours2, hierarchy2 = cv2.findContours(eye,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
        #hierarchy2 = hierarchy2[0]
        #for eye in range(len(contours2)):
        #print len(contours2)
        eyecount = 0
        for eye in range(len(contours2)):
            #print len(contours2)
            cnt = contours2[eye]
            area = cv2.contourArea(cnt)
            perimeter = cv2.arcLength(cnt, True)
            if perimeter >1:
                factor = 4 * math.pi * area / perimeter**2
                #print area, factor
                if area >150 and factor>0.8:
                    eyecount = eyecount + 1
        dices.append(eyecount)
        #print(eyecount)
    dices.sort()
    print dices
 

    #cv2.imshow('frame',frame)
    #print k    
    k = cv2.waitKey(0)
    print k
    #destroy
    cv2.destroyAllWindows()


