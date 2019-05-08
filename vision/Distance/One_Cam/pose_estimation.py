import numpy as np
import cv2
import glob
import os
import argparse

parser = argparse.ArgumentParser(description='Generate camera matrix and '
                                             'distortion parameters from checkerboard '
                                             'images')
parser.add_argument('images', help='path to images')

args = parser.parse_args()

# This function draws lines joining the given image points to the first chess board corner
def draw(img, corners, imgpts):
    imgpts = np.int32(imgpts).reshape(-1,2)
    # draw ground floor in green
    img = cv2.drawContours(img, [imgpts[:4]],-1,(0,255,0),-3)
    # draw pillars in blue color
    for i,j in zip(range(4),range(4,8)):
        img = cv2.line(img, tuple(imgpts[i]), tuple(imgpts[j]),(255),3)
    # draw top layer in red color
    img = cv2.drawContours(img, [imgpts[4:]],-1,(0,0,255),3)
    return img

# Load the camera calibration data
with np.load('pictures/camera_calibration/calib.npz') as calibData:
    mtx, dist, rvecs, tvecs = [calibData[i] for i in ('mtx', 'dist', 'rvecs', 'tvecs')]

# Define the chess board rows and columns
rows = 8
cols = 6

# Set the termination criteria for the corner sub-pixel algorithm
criteria = (cv2.TERM_CRITERIA_MAX_ITER + cv2.TERM_CRITERIA_EPS, 30, 0.001)

# Prepare the object points: (0,0,0), (1,0,0), (2,0,0), ..., (6,5,0). They are the same for all images
objectPoints = np.zeros((rows * cols, 1, 3), np.float32)
objectPoints[:, :, :2] = np.mgrid[0:rows, 0:cols].T.reshape(-1, 1, 2)

# Create the axis points
axisPoints = np.float32([[0,0,0], [0,3,0], [3,3,0], [3,0,0],
                   [0,0,-3],[0,3,-3],[3,3,-3],[3,0,-3] ])
extensions = ['jpg', 'JPG', 'png']

if os.path.isdir(args.images):
    img_names = [fn for fn in os.listdir(args.images)
                 if any(fn.endswith(ext) for ext in extensions)]
    proj_root = args.images

# Loop over the image files
for path in img_names:
    # Load the image and convert it to gray scale
    img = cv2.imread(os.path.join(proj_root, path))
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    
    # Find the chess board corners
    ret, corners = cv2.findChessboardCorners(gray, (rows, cols), None)
    
    # Make sure the chess board pattern was found in the image
    if ret:
        # Refine the corner position
        corners = cv2.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)
        
        # Find the rotation and translation vectors
        val, rvecs, tvecs, inliers = cv2.solvePnPRansac(objectPoints, corners, mtx, dist)
        
        # Project the 3D axis points to the image plane
        axisImgPoints, jac = cv2.projectPoints(axisPoints, rvecs, tvecs, mtx, dist)
        
        # Draw the axis lines
        img = draw(img, corners, axisImgPoints)
    
    # Display the image
    cv2.imshow('chess board', img)
    cv2.waitKey(0)

cv2.destroyAllWindows()
