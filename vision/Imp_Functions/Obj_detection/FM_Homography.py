import numpy as np
import cv2
import glob


def typesafe_perspectiveTransform(A,B):
    return cv2.perspectiveTransform(A.astype(np.float32, copy=False), B)


cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 240)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)
MIN_MATCH_COUNT = 5

# images = [cv2.imread(file) for file in glob.glob('../img/*jpg')]  # trainImage

images = []
images.append(cv2.imread('../img/kip_img6.jpg'))

# Initiate SIFT detector
sift = cv2.xfeatures2d.SIFT_create()

while True:
    _, frame = cap.read()
    # frame = cv2.bilateralFilter(frame, 9, 75, 75)
    kp1, des1 = sift.detectAndCompute(frame, None)

    for img in images:
        # find the keypoints and descriptors with SIFT
        # img = cv2.bilateralFilter(img, 9, 75, 75)
        kp2, des2 = sift.detectAndCompute(img, None)


        FLANN_INDEX_KDTREE = 0
        index_params = dict(algorithm=FLANN_INDEX_KDTREE, trees=100)
        search_params = dict(checks=1000)

        flann = cv2.FlannBasedMatcher(index_params, search_params)

        matches = flann.knnMatch(des1, des2, k=2)
        print matches
        break
        # store all the good matches as per Lowe's ratio test.
        good = []
        for m, n in matches:
            if m.distance < 0.7*n.distance:
                good.append(m)

        if len(good) > MIN_MATCH_COUNT:
            print("Enough matches are found - %d/%d" % (len(good), MIN_MATCH_COUNT))

            src_pts = np.float32([kp1[m.queryIdx].pt for m in good]).reshape(-1, 1, 2)
            dst_pts = np.float32([kp2[m.trainIdx].pt for m in good]).reshape(-1, 1, 2)

            M, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)
            matchesMask = mask.ravel().tolist()

            if M is not None:
                h, w, _ = frame.shape
                pts = np.float32([[0, 0], [0, h - 1], [w - 1, h - 1], [w - 1, 0]]).reshape(-1, 1, 2)
                dst = cv2.perspectiveTransform(pts, M)
            else:
                dst = []

            frame = cv2.polylines(frame, [np.int32(dst)], True, (255, 255, 255), 3, cv2.LINE_AA)

        else:
            print("Not enough matches are found - %d/%d" % (len(good), MIN_MATCH_COUNT))
            matchesMask = None


        draw_params = dict(matchColor=(0, 255, 0),  # draw matches in green color
                           singlePointColor=None,
                           matchesMask=matchesMask,  # draw only inliers
                           flags=2)

        matching_result = cv2.drawMatches(frame, kp1, img, kp2, good, None, **draw_params)
        cv2.imshow('Matching_result', matching_result)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
