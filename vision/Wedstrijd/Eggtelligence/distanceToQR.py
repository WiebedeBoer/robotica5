import cv2
import numpy as np

cam = cv2.VideoCapture(0)

# Initialize the known object width, which in this case
KNOWN_WIDTH = 15.0
# Initialize the known object focal length, which in this case
FOCAL_LENGTH = 1000

KNOWN_DISTANCE = 100


def main(object):
    _, frame = cam.read()

    x, y, w, h = object.rect
    distance = calculateDistance(w)
    return distance


def calculateDistance(w):
    return (FOCAL_LENGTH * (KNOWN_WIDTH / 2)) / (w / 2)


def calibration(w):
    return (KNOWN_DISTANCE * (w / 2)) / (KNOWN_WIDTH / 2)




