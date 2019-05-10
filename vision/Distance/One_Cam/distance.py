import cv2
import numpy as np
import os, sys
import math
from decimal import *
import glob

cam = cv2.VideoCapture(0)

cam.set(3, 720)
cam.set(4, 620)

# Distance calculating
# Initialize the known distance from the camera to the object
KNOWN_DISTANCE = 100.0
# Initialize the known object width, which in this case
KNOWN_WIDTH = Decimal(15.0)
# Initialize the known object focal length, which in this case
FOCAL_LENGTH = None


# Text settings
font                   = cv2.FONT_HERSHEY_SIMPLEX
bottomLeftCornerOfText = (10,500)
fontScale              = 0.75
fontColor              = (255,255,255)
lineType               = 2

text = None

# Add color blue to mask
boundaries = [
              [([20, 100, 100], [30, 255, 255])], #Yellow
              #[([100,150,0], [140,255,255])], #Blue
              #[([20, 100, 100], [30, 255, 255])] #Red
         ]

totalContours = []


def main():
    while (1):
        _, frame = cam.read()
        img1_bg = frame
        img = cv2.GaussianBlur(frame, (15, 15), 0)
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        grayFrame = CLAHE(gray)

        mask = np.ones(img.shape[:2], dtype="uint8") * 255

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
                perimeter = cv2.arcLength(cnt, True)
                approx = cv2.approxPolyDP(cnt, 0.04 * perimeter, True)
                x, y, w, h = cv2.boundingRect(cnt)
                
                if(w > 40 and h > 10 and len(approx) >= 4 and len(approx) <= 6):
                    #frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 255), 2)

                    cv2.drawContours(mask, [cnt], -1, 0, -1)
                    _, mask = cv2.threshold(mask, 10, 255, cv2.THRESH_BINARY_INV)

                    img1_bg = cv2.bitwise_and(grayFrame, grayFrame, mask=mask)
                    img1_bg = cv2.GaussianBlur(img1_bg, (15, 15), 0)
                    img1_bg = cv2.adaptiveThreshold(img1_bg, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11,2)

                    leftmost = tuple(cnt[cnt[:, :, 0].argmin()][0])
                    rightmost = tuple(cnt[cnt[:, :, 0].argmax()][0])
                    topmost = tuple(cnt[cnt[:, :, 1].argmin()][0])
                    bottommost = tuple(cnt[cnt[:, :, 1].argmax()][0])

                    print("left: " + str(leftmost) + " " + "right: " + str(rightmost) + " " + "top: " + str(topmost) + " " + "bottom: " + str(bottommost))

                    if w > (h*1.6):
                        KNOWN_WIDTH = 25.0
                        FOCAL_LENGTH = 1024
                        text = "Side"
                    elif w < (h*1.6):
                        KNOWN_WIDTH = 15
                        FOCAL_LENGTH = 1180
                        text = "Voorkant"
                    else:
                        text = "Unkown"

                    # Distance to object calculating
                    distance = calculateDistance(w, KNOWN_WIDTH, FOCAL_LENGTH)

                    # Apply text in frame
                    cv2.putText(frame,str(str(w)+ " " + str(h) + " " + str(round(distance, 2))) + " " + text,
                                (x + (w / 2), y),font,fontScale,fontColor,lineType)
        del totalContours[:]
                
        #Show frame
        cv2.imshow('images', frame)
        cv2.imshow('img1_bg', img1_bg)
        
        #Check if "esc" is pressed
        k = cv2.waitKey(1) & 0xFF
        if k == 27:
            break
    
    cv2.destroyAllWindows()


def calibration(w):
    return (KNOWN_DISTANCE * (w / 2)) / (KNOWN_WIDTH / 2)


def calculateDistance(w, KNOWN_WIDTH, FOCAL_LENGTH):
    print(KNOWN_WIDTH)
    return (FOCAL_LENGTH * (KNOWN_WIDTH / 2)) / (w / 2)


def CLAHE(frame):
    clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
    cl1 = clahe.apply(frame)

    return cl1

main()


