import cv2
import numpy as np
from decimal import *

cam = cv2.VideoCapture(0)
cam.set(3, 640)
cam.set(4, 480)

# Define the chess board rows and columns
rows = 8
cols = 6

# Load the camera calibration data
with np.load('pictures/camera_calibration/calib.npz') as calibData:
    mtx, dist, rvecs, tvecs = [calibData[i] for i in ('mtx', 'dist', 'rvecs', 'tvecs')]

# Initialize the known distance from the camera to the object
KNOWN_DISTANCE = 100.0
# Initialize the known object width, which in this case
KNOWN_WIDTH = Decimal(15.0)
# Initialize the known object focal length, which in this case
FOCAL_LENGTH = 1013

# Text settings
font = cv2.FONT_HERSHEY_SIMPLEX
bottomLeftCornerOfText = (10, 500)
fontScale = 0.75
fontColor = (255, 255, 255)
lineType = 2

# Add color blue to mask
boundaries = [
    [([20, 100, 100], [30, 255, 255])],  # Yellow
    # [([100, 150, 0], [140, 255, 255])],  # Blue
    [([20, 100, 100], [30, 255, 255])]  # Red
]

totalContours = []

# Set the termination criteria for the corner sub-pixel algorithm
criteria = (cv2.TERM_CRITERIA_MAX_ITER + cv2.TERM_CRITERIA_EPS, 30, 0.001)

# Create the axis points
axisPoints = np.float32([[0, 0, 0], [0, 3, 0], [3, 3, 0], [3, 0, 0],
                         [0, 0, -3], [0, 3, -3], [3, 3, -3], [3, 0, -3]])


def main():
    while (1):
        _, frame = cam.read()
        img = cv2.GaussianBlur(frame, (15, 15), 0)
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
                area = cv2.contourArea(cnt)
                perimeter = cv2.arcLength(cnt, True)
                approx = cv2.approxPolyDP(cnt, 0.04 * perimeter, True)
                x, y, w, h = cv2.boundingRect(cnt)

                if (w > 40 and h > 10 and len(approx) >= 4 and len(approx) <= 6):
                    # Set contour arround object
                    frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 255), 2)
                    print("In latest if statement")

                    #objectPoints = np.zeros((rows * cols, 1, 3), np.float32)
                    #objectPoints[:, :, :2] = np.mgrid[0:rows, 0:cols].T.reshape(-1, 1, 2)

                    frame = pose_estimation(frame, approx, area)

                    # Distance to object calculating
                    distance = calculateDistance(w)

                    # Apply text in frame
                    cv2.putText(frame, str(str(w) + " " + str(h) + " " + str(round(distance, 2))),
                                (x + (w / 2), y), font, fontScale, fontColor, lineType)

        del totalContours[:]

        # Show frame
        cv2.imshow('camera', frame)

        # Check if "esc" is pressed
        k = cv2.waitKey(1) & 0xFF
        if k == 27:
            break

    cv2.destroyAllWindows()


def calibration(w):
    return (KNOWN_DISTANCE * (w / 2)) / (KNOWN_WIDTH / 2)


def calculateDistance(w):
    return (FOCAL_LENGTH * (KNOWN_WIDTH / 2)) / (w / 2)


# This function draws lines joining the given image points to the first chess board corner
def draw(frame, corners, imgpts):
    imgpts = np.int32(imgpts).reshape(-1, 2)
    # draw ground floor in green
    frame = cv2.drawContours(frame, [imgpts[:4]], -1, (0, 255, 0), -3)
    # draw pillars in blue color
    for i, j in zip(range(4), range(4, 8)):
        frame = cv2.line(frame, tuple(imgpts[i]), tuple(imgpts[j]), (255), 3)
    # draw top layer in red color
    frame = cv2.drawContours(frame, [imgpts[4:]], -1, (0, 0, 255), 3)
    return frame


def pose_estimation(frame, corners, objectPoints):
    # Load the image and convert it to gray scale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Make sure the chess board pattern was found in the image
    # Refine the corner position
    corners = cv2.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)

    # Find the rotation and translation vectors
    val, rvecs, tvecs, inliers = cv2.solvePnPRansac(objectPoints, corners, mtx, dist)

    # Project the 3D axis points to the image plane
    axisImgPoints, jac = cv2.projectPoints(axisPoints, rvecs, tvecs, mtx, dist)

    # Draw the axis lines
    frame = draw(frame, corners, axisImgPoints)
    return frame


main()
