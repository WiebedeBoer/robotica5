import sys
sys.path.append('Imp_Functions/')

from fm_ORB import fm_ORB
import cv2


def chickenSurvivalRun(frame):
    imgTrainColor = cv2.imread('Images/TrainImg/Beker/beker1.jpg')
    return fm_ORB(frame, imgTrainColor)