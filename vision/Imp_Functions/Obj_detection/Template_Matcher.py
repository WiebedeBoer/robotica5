import cv2
import numpy as np
from matplotlib import pyplot as plt
import time

cap = cv2.VideoCapture(0)

template = cv2.imread('../img/png/kip_head.png')
w, h, _ = template.shape[::-1]
time.sleep(1)

while True:
    print("In True")
    _, frame = cap.read()
    # All the 6 methods for comparison in a list
    # methods = ['cv2.TM_CCOEFF_NORMED', 'cv2.TM_CCORR_NORMED', 'cv2.TM_SQDIFF_NORMED']

    methods = ['cv2.TM_CCOEFF_NORMED']

    for meth in methods:
        method = eval(meth)

        # Apply template Matching
        res = cv2.matchTemplate(frame, template, method)
        min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)

        # If the method is TM_SQDIFF or TM_SQDIFF_NORMED, take minimum
        if method in [cv2.TM_SQDIFF_NORMED]:
            top_left = min_loc
        else:
            top_left = max_loc

        bottom_right = (top_left[0] + w, top_left[1] + h)
        # cv2.rectangle(frame, top_left, bottom_right, 255, 2)
        cv2.rectangle(frame, (top_left[0], top_left[1]), (top_left[0] + w, top_left[1] + h), (0, 255, 0), 2)

        cv2.imshow(str(meth), frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindow()