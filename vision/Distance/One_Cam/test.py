import cv2
import numpy as np

cam = cv2.VideoCapture(0)
boundaries = [
              ([ 15, 183, 116], [ 35, 203, 196]),
              ([ 26, 123, 164], [ 36, 133, 204])
              ]

# loop over the boundaries
lower_yellow = np.array([16, 133, 121])
upper_yellow = np.array([36, 153, 201])


while (True):
    _, frame = cam.read()
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
    
    for (lower, upper) in boundaries:
        lower = np.array(lower, dtype = "uint8")
        upper = np.array(upper, dtype = "uint8")

        #find the colors within the specified boundaries and apply the mask
        in_mask = cv2.inRange(hsv, lower, upper)
        mask = mask + in_mask

    output = cv2.bitwise_and(frame, frame, mask = mask)
    cv2.imshow('images', output)

    #mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
    #res = cv2.bitwise_and(frame,frame, mask= mask)

    #kernel = np.ones((15, 15), np.float32)/225
    #smooted = cv2.filter2D(res, -1, kernel)

    #cv2.imshow('camera', res)
    #cv2.imshow('smooted', smooted)
    k = cv2.waitKey(1) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()
