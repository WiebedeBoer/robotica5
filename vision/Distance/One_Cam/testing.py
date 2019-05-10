import cv2
import numpy as np
from decimal import *

cam = cv2.VideoCapture(0)
cam.set(3, 720)
cam.set(4, 620)

# Add color blue to mask
boundaries = [
    [([20, 100, 100], [30, 255, 255])],  # Yellow
    # [([100, 150, 0], [140, 255, 255])],  # Blue
    # [([20, 100, 100], [30, 255, 255])]  # Red
]

totalContours = []

# Prepare the object points: (0,0,0), (1,0,0), (2,0,0), ..., (6,5,0). They are the same for all images
objectPoints = np.zeros((rows * cols, 1, 3), np.float32)
objectPoints[:, :, :2] = np.mgrid[0:rows, 0:cols].T.reshape(-1, 1, 2)


# Create the axis points
axisPoints = np.float32([[0,0,0], [0,3,0], [3,3,0], [3,0,0],
                   [0,0,-3],[0,3,-3],[3,3,-3],[3,0,-3] ])


def main():
    while (1):
        _, frame = cam.read()

        img1_bg = frame
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        grayFrame = CLAHE(gray)
        img = cv2.GaussianBlur(frame, (15, 15), 0)

        mask = np.ones(img.shape[:2], dtype="uint8") * 255


        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

        for range in boundaries:
            in_mask = None
            for (lower, upper) in range:
                # Find the colors within the specified boundaries and apply the mask
                if (in_mask is None):
                    in_mask = cv2.inRange(hsv, np.array(lower, dtype="uint8"), np.array(upper, dtype="uint8"))
                else:
                    in_mask = in_mask + cv2.inRange(hsv, np.array(lower, dtype="uint8"), np.array(upper, dtype="uint8"))
                _, res = cv2.threshold(in_mask, 215, 255, cv2.THRESH_BINARY)
                res = cv2.GaussianBlur(res, (15, 15), 0)
                contours, hierarchy = cv2.findContours(res, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
                totalContours.append(contours)

        for contours in totalContours:
            for cnt in contours:
                perimeter = cv2.arcLength(cnt, True)
                approx = cv2.approxPolyDP(cnt, 0.04 * perimeter, True)
                x, y, w, h = cv2.boundingRect(cnt)

                if (w > 40 and h > 10 and len(approx) >= 4 and len(approx) <= 6):
                    cv2.drawContours(mask, [cnt], -1, 0, -1)
                    _, mask = cv2.threshold(mask, 10, 255, cv2.THRESH_BINARY_INV)

                    img1_bg = cv2.bitwise_and(grayFrame, grayFrame, mask=mask)

                    img1_bg = cv2.GaussianBlur(img1_bg, (15, 15), 0)

                    img1_bg = cv2.adaptiveThreshold(img1_bg, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)

                    # Set contour around object
                    # frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 255), 2)

                    smallFrame = np.float32(img1_bg)
                    dst = cv2.cornerHarris(smallFrame, 2, 3, 0.05)

                    # result is dilated for marking the corners, not important
                    dst = cv2.dilate(dst, None)

                    # Threshold for an optimal value, it may vary depending on the image.
                    frame[dst > 0.01 * dst.max()] = [0, 0, 255]

        del totalContours[:]

        cv2.imshow('camera', frame)

        if (cv2.waitKey(1) & 0xFF) == 27:
            break

    cv2.destroyAllWindows()

def CLAHE(frame):
    clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
    cl1 = clahe.apply(frame)

    return cl1

main()
