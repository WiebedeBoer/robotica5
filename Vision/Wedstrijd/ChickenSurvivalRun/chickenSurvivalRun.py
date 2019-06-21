import sys
sys.path.append('Imp_Functions/')

from fm_ORB import fm_ORB
from helpFunctions import *
import cv2

def chickenSurvivalRun(frame):
    imgTrainColor = cv2.imread('Images/TrainImg/Beker/piBeker.jpg')
    rectanglePts = fm_ORB(frame, imgTrainColor, 30)

    if rectanglePts is not False and rectanglePts is not None:
        center = calculateCenter(rectanglePts)
        xyAxis = whichDirection(center[0], center[1])
        return str("Unknown" + ':' + str(xyAxis[0]) + ':' + str(xyAxis[1]))   # Returns distance, x-axis, y-axis
    else:
        return False
