import cv2
import numpy as np
import math

def ViewBeam():
    cap = cv2.VideoCapture(0)
    while(1):
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
            if(area >100 and area <3600 and factor >0 and factor <0.99):
                frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 0), 2)
                #print command
                print("FollowBeam;")
            elif(area >3600 and factor >0 and factor <0.99):
                frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)
                #print command
                print("StopBeam;")
            else:
                #print command
                print("SearchBeam;")

        #show vid
        cv2.imshow("Frame", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    cap.release()
    cv2.destroyAllWindows()

ViewBeam()
