import numpy as np
import cv2

# Load the image
cap = cv2.VideoCapture(0)
img = cv2.imread('../img/kip_img1.jpg')


while True:
    _, frame = cap.read()

    # Convert it to gray scale
    gray1 = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    gray2 = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # Detect the CenSurE key points
    star = cv2.xfeatures2d.StarDetector_create()
    keyPointsFrame = star.detect(gray1, None)
    keyPointsImg = star.detect(gray2, None)

    # Create the BRIEF extractor and compute the descriptors
    brief = cv2.xfeatures2d.BriefDescriptorExtractor_create()
    keyPoints1, descriptors1 = brief.compute(frame, keyPointsFrame)
    keyPoints2, descriptors2 = brief.compute(img, keyPointsImg)

    # Paint the key points over the original image
    result1 = cv2.drawKeypoints(frame, keyPoints1, None, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    result2 = cv2.drawKeypoints(img, keyPoints2, None, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

       bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)
    matches = bf.match(descriptors1, descriptors2)
    matches = sorted(matches, key=lambda x: x.distance)

    # Draw matches
    matching_result = cv2.drawMatches(img, keyPoints1, frame, keyPoints2, matches[:20], None, flags=2)

    # Display the results
    cv2.imshow('Key points', matching_result)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.waitKey(0)
cv2.destroyAllWindows()