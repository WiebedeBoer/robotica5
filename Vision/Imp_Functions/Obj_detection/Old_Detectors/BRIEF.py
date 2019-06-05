import numpy as np
import cv2

# Load the image
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

img = cv2.imread('../img/png/kip_img1-min.png')
dim = (640, 480)
img = cv2.resize(img, dim, interpolation = cv2.INTER_AREA)


while True:
    # Detect the CenSurE key points
    star = cv2.xfeatures2d.StarDetector_create()
    keyPointsImg = star.detect(img, None)

    # Create the BRIEF extractor and compute the descriptors
    brief = cv2.xfeatures2d.BriefDescriptorExtractor_create()
    keyPoints2, descriptors2 = brief.compute(img, keyPointsImg)

    # Paint the key points over the original image
    result2 = cv2.drawKeypoints(img, keyPoints2, None, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

    # Display the results
    cv2.imshow('Key points', result2)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.waitKey(0)
cv2.destroyAllWindows()