import cv2
import numpy as np
import math

def con():
    frame = cv2.imread('C:/Users/wiebe/Documents/robotica/opencv_workshop_materiaal(4)/bouten_moeren.jpg')
    #hsv
    #hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray,(5,5),0)
    ret, th = cv2.threshold(blur,180,255,cv2.THRESH_BINARY_INV)
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
    for cnr in range(len(contours)):
        cnt = contours[cnr]
        area = cv2.contourArea(cnt)
        perimeter = cv2.arcLength(cnt, True)
        factor = 4 * math.pi * area / perimeter**2
        holes = 0
        child = hierarchy[cnr][2]
        while child >= 0:
            holes += cv2.contourArea(contours[child])
            child = hierarchy[child][0]
        print area, factor, holes

        if area >3000:
            #yellow liggend bout, vormfactor <1
            cv2.drawContours(frame, [cnt], -1, (0,255,255), 3)
        else:
            if holes >0:
                #blue liggen moer, cirkel vormfactor =1, contour hierarchy groot gat
                cv2.drawContours(frame, [cnt], -1, (255,0,0), 3)
            else:
                if factor <0.88 and area <2000:
                    #red staand bout, vormfactor <1
                    cv2.drawContours(frame, [cnt], -1, (0,0,255), 3)
                elif factor <0.88 and area >2000:
                    #green staand moer, vormfactor <1
                    cv2.drawContours(frame, [cnt], -1, (0,255,0), 3)


    
    #for cnt in contours:
        #area = cv2.contourArea(cnt)
        #if area > 100:
            #vormfactor = 4 * pi * opp / omtrek2
            #yellow liggen moer, cirkel vormfactor =1, contour hierarchy groot gat
            #cv2.drawContours(frame, [cnt], -1, (0,255,255), 3)
            #red staand bout, vormfactor <1
            #cv2.drawContours(frame, [cnt], -1, (0,255,255), 3)
            #green staand moer, vormfactor <1
            #cv2.drawContours(frame, [cnt], -1, (0,255,255), 3)
            #blue liggend bout, vormfactor <1
            #cv2.drawContours(frame, [cnt], -1, (0,255,255), 3)    
    #contours    
    #cnt = contours[0]    
    #M = cv2.moments(cnt)
    #cnt = cv2.convexHull(cnt)
    #cy = int(M['m01']/M['m00'])
    #cx = int(M['m10']/M['m00'])
    #area = cv2.contourArea(cnt)
    #perimeter = cv2.arcLength(cnt,True)
    #draw contours
    #cv2.drawContours(frame, [cnt], -1, (0,255,0), 3)
    #draw a circle
    #cv2.circle(frame,(cx,cy), 5, (0,255,255), -1)    
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


