import numpy as np
import cv2
import math
def test1():
    img = cv2.imread('C:/Users/wiebe/Documents/robotica/opencv_workshop_materiaal(4)/opencv-logo-white.png')
    cv2.imshow('logo', img)
    k = cv2.waitKey(0)
    print k
    cv2.destroyAllWindows()
