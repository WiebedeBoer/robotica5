# Calculate distance and return the distance
def calculateDistance(w, focal_length, known_width):
    return (focal_length * (known_width / 2)) / (w / 2)


# Calibrate and return the focal length
def calibration(w, known_distance, known_width):
    return (known_distance * (w / 2)) / (known_width / 2)


# Calculate if the robot has to go left or right
def dirLeftOrRight(xMidPoint):
    turn_x = xMidPoint - (640 / 2)  # 640 is the width of the frame
    return turn_x


# Calculate if the robot has to go top or bottom
def dirTopOrBottom(yMidPoint):
    turn_y = yMidPoint - (480 / 2)  # 480 is the height of the frame
    return turn_y

