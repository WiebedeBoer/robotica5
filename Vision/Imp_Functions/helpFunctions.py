import cv2


# Calculate distance and return the distance
def calculateDistance(w, focal_length, known_width):
	return round((focal_length * (known_width / 2)) / (w / 2), 2)


# Calibrate and return the focal length
def calibration(w, known_distance, known_width):
	return round((known_distance * (w / 2)) / (known_width / 2), 2)


# Calculate if the robot has to go left, right, top or bottom
def whichDirection(xMidPoint, yMidPoint):
	turn_x = xMidPoint - (640 / 2)  # 640 is the width of the frame
	turn_y = (480 / 2) - yMidPoint  # 480 is the height of the frame
	return turn_x, turn_y


def CLAHE(frame):
	clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
	return clahe.apply(frame)


# Calculates the center of points
def calculateCenter(pts):
	allXPoints = []
	allYPoints = []

	for p in pts:
		allXPoints.append(p[0])
		allYPoints.append(p[1])

	sumX = 0
	for x in allXPoints:
		sumX += x

	sumY = 0
	for y in allYPoints:
		sumY += y
	return [sumX / len(pts), sumY / len(pts)]
