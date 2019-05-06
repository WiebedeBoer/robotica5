import cv2
import numpy as np
import os, sys
import math

cam = cv2.VideoCapture(0)

# Distance calculating
# Initialize the known distance from the camera to the object
KNOWN_DISTANCE = 100.0
# Initialize the known object width, which in this case
KNOWN_WIDTH = 15.0
# Initialize the known object focal length, which in this case
FOCAL_LENGTH = 1013

# Text settings
font = cv2.FONT_HERSHEY_SIMPLEX
bottomLeftCornerOfText = (10, 500)
fontScale = 0.75
fontColor = (255, 255, 255)
lineType = 2

# Add color blue to mask
boundaries = [
    ([100, 150, 0], [140, 255, 255])
]

# Loop over the boundaries
lower_yellow = np.array([20, 100, 100])
upper_yellow = np.array([30, 255, 255])


def main():
    f = open("results.csv", "w+")
    while (True):
        _, frame = cam.read()
        img = cv2.GaussianBlur(frame, (15, 15), 0)
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
        for (lower, upper) in boundaries:
            lower = np.array(lower, dtype="uint8")
            upper = np.array(upper, dtype="uint8")
            # Find the colors within the specified boundaries and apply the mask
            in_mask = cv2.inRange(hsv, lower, upper)
            mask = mask + in_mask

        _, res = cv2.threshold(mask, 215, 255, cv2.THRESH_BINARY)
        res = cv2.GaussianBlur(res, (15, 15), 0)
        cv2.imshow('Trunc', res)
        contours, hierarchy = cv2.findContours(res, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        for cnt in contours:
            area = cv2.contourArea(cnt)
            perimeter = cv2.arcLength(cnt, True)
            x, y, w, h = cv2.boundingRect(cnt)
            approx = cv2.approxPolyDP(cnt, 0.04 * perimeter, True)
            if(perimeter > 0 and w > 50 and h > 20 and len(approx) >= 4 and len(approx) <= 6 and ((w/h > 0.84 and w/h < 1.24) or (w/h > 0.714 and w/h < 0.805) or (w/h > 1.435 and w/h < 1.639))):
                vormfactor = 4 * math.pi * area / perimeter ** 2
                if(vormfactor > 0.5 and vormfactor < 0.82):
                     # Set contour arround object
                    frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 255), 2)

                    # Distance to object calculating
                    distance = calculateDistance(w)

                    # (312 - 160) / 50 = 3.04
                    # 464 - (3.04 * distance)
                    # 0.048 / 50 = (3 / 3125)
                    relLength = (3 / 3125) * distance  # Calculating relatieve length
                    KNOWN_WIDTH = (((w + distance * 3.04) - (
                                3.04 * distance)) * relLength)  # First = calculation at 0 cm // Second = calc what width is at that distance

                    # Calibration
                    # focalLength = calibration(w)

                    # Apply text in frame
                    cv2.putText(frame, str(str(w) + " " + str(KNOWN_WIDTH) + " " + str(h) + " " + str(distance)),
                                (x + w, y + h),
                                font,
                                fontScale,
                                fontColor,
                                lineType)

        # Show frame
        cv2.imshow('images', frame)
        # Check if "esc" is pressed
        k = cv2.waitKey(1) & 0xFF
        if k == 27:
            break

    f.close()
    cv2.destroyAllWindows()


def calibration(w):
    return (KNOWN_DISTANCE * (w / 2)) / (KNOWN_WIDTH / 2)


def calculateDistance(w):
    return (FOCAL_LENGTH * (KNOWN_WIDTH / 2)) / (w / 2)


main()
