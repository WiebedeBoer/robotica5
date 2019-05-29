import cv2
import numpy as np
import glob

images = [cv2.imread(file) for file in glob.glob('../img/*jpg')]

print(len(images))

cap = cv2.VideoCapture(0)

# ORB Detection
orb = cv2.ORB_create()


while True:
    _, frame = cap.read()
    kp2, des2 = orb.detectAndCompute(frame, None)

    for img in images:
        # img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        kp1, des1 = orb.detectAndCompute(img, None)

        # Brute force matching
        bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)
        matches = bf.match(des1, des2)
        matches = sorted(matches, key=lambda x:x.distance)

        # Draw matches
        matching_result = cv2.drawMatches(img, kp1, frame, kp2, matches[:20], None, flags=2)

        cv2.imshow('Matching_Result', matching_result)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

cv2.destroyAllWindows()
