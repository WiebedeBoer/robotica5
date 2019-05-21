import cv2
import numpy as numpy
import matplotlib.pyplot as plt

# Wait for socket
#sok.recv(1024,0)

img1 = cv2.imread('kip.png',0) #Training
img2 = cv2.imread('kiporg2.jpg',0) #Identify
#img2 = cv2.VideoCapture('groenbakje.h264',0)
video = cv2.VideoCapture('kip_afstand.h264')


orb = cv2.ORB_create()

kp1, des1 = orb.detectAndCompute(img1,None)
kp2, des2 = orb.detectAndCompute(img2,None)

bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck = True)

matches = bf.match(des1, des2)
matches = sorted(matches, key=lambda x:x.distance)

img3 = cv2.drawMatches(img1,kp1,img2,kp2, matches[:10], None, flags=2)
plt.imshow(img3)
plt.show()
