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
KNOWN_DISTANCE = 200.0
# Initialize the known object width, which in this case
KNOWN_WIDTH = Decimal(15.0)
# Initialize the known object focal length, which in this case
FOCAL_LENGTH = None

# Text settings
font = cv2.FONT_HERSHEY_SIMPLEX
bottomLeftCornerOfText = (10, 500)
fontScale = 0.75
fontColor = (255, 255, 255)
lineType = 2

text = None

# Add color blue to mask
boundaries = [
    [([20, 100, 100], [30, 255, 255])],  # Yellow
    # [([100,150,0], [140,255,255])], #Blue
    # [([20, 100, 100], [30, 255, 255])] #Red
]

totalContours = []


def main():
    while (1):
        _, frame = cam.read()
        img = cv2.bilateralFilter(frame, 9, 75, 75)
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        grayFrame = CLAHE(gray)

        for range in boundaries:
            in_mask = None;
            for (lower, upper) in range:
                # Find the colors within the specified boundaries and apply the mask
                if (in_mask is None):
                    in_mask = cv2.inRange(hsv, np.array(lower, dtype="uint8"), np.array(upper, dtype="uint8"));
                else:
                    in_mask = in_mask + cv2.inRange(hsv, np.array(lower, dtype="uint8"), np.array(upper, dtype="uint8"))
                _, res = cv2.threshold(in_mask, 215, 255, cv2.THRESH_BINARY)

                res = cv2.bitwise_and(res, res, mask=grayFrame)
                contours, hierarchy = cv2.findContours(res, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
                totalContours.append(contours)

        for contours in totalContours:
            for cnt in contours:
                perimeter = cv2.arcLength(cnt, True)
                approx = cv2.approxPolyDP(cnt, 0.04 * perimeter, True)
                x, y, w, h = cv2.boundingRect(cnt)

                if (w > 40 and h > 10 and len(approx) >= 4 and len(approx) <= 6):
                    # Set contour arround object
                    frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 255), 2)

                    if w > (h * 1.725):
                        KNOWN_WIDTH = 25.0
                        text = "Side"
                    elif w >= (h * 1.4) and w <= (h * 1.725):
                        KNOWN_WIDTH = 30.0
                        text = "SideAngle"
                    elif w < (h * 1.4):
                        KNOWN_WIDTH = 15
                        text = "Voorkant"

                    # Calibration
                    focal = calibration(w, KNOWN_WIDTH)
                    print(str(str(focal) + " = " + str(text)))

                    # Apply text in frame
                    cv2.putText(frame, str(str(w) + " " + str(h) + " " + str(text)),
                                (x + (w / 2), y),
                                font,
                                fontScale,
                                fontColor,
                                lineType)
        del totalContours[:]

        # Show frame
        cv2.imshow('images', frame)

        # Check if "esc" is pressed
        if (cv2.waitKey(1) & 0xFF) == 27:
            break

    cv2.destroyAllWindows()


def calibration(w, KNOWN_WIDTH):
    return (KNOWN_DISTANCE * (w / 2)) / (KNOWN_WIDTH / 2)


def calculateDistance(w, KNOWN_WIDTH, FOCAL_LENGTH):
    print(KNOWN_WIDTH)
    return (FOCAL_LENGTH * (KNOWN_WIDTH / 2)) / (w / 2)


def CLAHE(frame):
    clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
    cl1 = clahe.apply(frame)

    return cl1


main()
