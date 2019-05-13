import numpy as np
import cv2
import pyzbar.pyzbar as pyzbar

def ViewBeam():
    cap = cv2.VideoCapture(0)
    while(1):
        _, frame = cap.read()
        #([100,150,0], [140,255,255])
        # go over the boundaries, blue beam
        lower_blue = np.array([100,150,0])
        upper_blue = np.array([140,255,255])
        #mask
        hsv = cv2.cvtColor(frame, cv2.COLOR_RGB2HSV)
        mask = cv2.inRange(hsv, lower_blue, upper_blue)
        #contours
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
            if(area >200 and area <3600 and factor >0 and factor <0.88):
                frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 0), 2)

        #show vid
        cv2.imshow("Frame", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    cap.release()
    cv2.destroyAllWindows()

ViewBeam()
