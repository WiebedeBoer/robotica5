import cv2
import numpy as np

cam = cv2.VideoCapture(0)
cam.set(3, 720)
cam.set(4, 620)

# Initialize the known object width, which in this case
KNOWN_WIDTH = 15.0
# Initialize the known object focal length, which in this case
FOCAL_LENGTH = None


def main(object):
    _, frame = cam.read()

    img = cv2.bilateralFilter(frame, 9, 75, 75)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    _, res = cv2.threshold(object, 215, 255, cv2.THRESH_BINARY)
    res = cv2.bitwise_and(res, res, mask=gray)

    contours, hierarchy = cv2.findContours(res, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    perimeter = cv2.arcLength(contours, True)
    approx = cv2.approxPolyDP(contours, 0.04 * perimeter, True)
    x, y, w, h = cv2.boundingRect(contours)

    cam.release()

    if(w > 40 and h > 10 and len(approx) >= 4):
        distance = calculateDistance(w)
        return distance


def calculateDistance(w):
    print(KNOWN_WIDTH)
    return (FOCAL_LENGTH * (KNOWN_WIDTH / 2)) / (w / 2)



