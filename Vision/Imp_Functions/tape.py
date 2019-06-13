import sys
import cv2
import numpy as np
import math


def slope(var):
	x1 = float( var[0] )
	y1 = float( var[1] )
	x2 = float( var[2] )
	y2 = float( var[3] )

	slopeY = 0
	slopeX = 0

	try:
		if(abs(y2 - y1) > 0):
			slopeY = ( (x2 - x1) / (y2 - y1) ) * 180.0 / np.pi

		if(abs(x2 - x1) > 0):
			slopeX = ( (y2 - y1) / (x2 - x1) ) * 180.0 / np.pi

		if(abs(slopeX) > 0 and abs(slopeX) < 100):
			return slopeX
		elif (abs(slopeY) > 0 and abs(slopeY) < 100):
			return slopeY
		else:
			if(abs(slopeX) < abs(slopeY)):
				return slopeX
			else:
				return slopeY

	except ZeroDivisionError:
		return 0

def isParallel(var1, var2, threshold):

	slope1 = round( float(slope( var1 )), 0 )
	slope2 = round( float(slope( var2 )), 0 )

	if slope1 - slope2 == 0:
		return True
	elif abs( slope1 - slope2 ) <= threshold:
		return True
	else:
		return False

def removeBlackBars(img):
	gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
	_,th = cv2.threshold(gray,1,255,cv2.THRESH_BINARY)

	contours,hierarchy = cv2.findContours(th,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
	cnt = contours[0]
	x,y,w,h = cv2.boundingRect(cnt)

	crop = img[y:y+h,x:x+w]
	return crop

def lineRect(x1, y1, x2, y2, rx, ry, rw, rh, img):

	x1 = float(x1)
	y1 = float(y1)
	x2 = float(x2)
	y2 = float(y2)
	rx = float(rx)
	ry = float(ry)
	rw = float(rw)
	rh = float(rh)

	# check if the line has hit any of the rectangle's sides
	# uses the Line/Line function below
	left =   lineLine(img, x1,y1,x2,y2, rx,ry,rx, ry+rh)
	right =  lineLine(img, x1,y1,x2,y2, rx+rw,ry, rx+rw,ry+rh)
	top =    lineLine(img, x1,y1,x2,y2, rx,ry, rx+rw,ry)
	bottom = lineLine(img, x1,y1,x2,y2, rx,ry+rh, rx+rw,ry+rh)

	# if ANY of the above are true, the line
	# has hit the rectangle
	if(left):
		cv2.line(img, (int( rx ), int( ry )), (int( rx ), int( ry + rh )), (0, 0, 255), 3)
	if(right):
		cv2.line(img, (int( rx + rw ), int( ry )), (int( rx + rw ), int( ry + rh )), (0, 0, 255), 3)
	if(top):
		cv2.line(img, (int( rx ), int( ry )), (int( rx + rw ), int( ry )), (0, 0, 255), 3)
	if(bottom):
		cv2.line(img, (int( rx ), int( ry + rh )), (int( rx + rw ), int( ry + rh )), (0, 0, 255), 3)

	if (left or right or top or bottom):
		direction = ""
		if (left and right):
			direction += "front "
		elif (left):
			direction += "left "
		elif (right):
			direction += "right "
		elif (bottom):
			direction += "HOLY FUK TURN AROUND "

		return direction
	return False

def lineLine(img, (x1, y1, x2, y2), (x3, y3, x4, y4)):

	x1 = float(x1)
	y1 = float(y1)
	x2 = float(x2)
	y2 = float(y2)

	x3 = float(x3)
	y3 = float(y3)
	x4 = float(x4)
	y4 = float(y4)


	# calculate the direction of the lines
	uA = None
	uB = None
	try:
		uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1))
		uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1))
	except ZeroDivisionError:
		return False

	# if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 and uA <= 1 and uB >= 0 and uB <= 1):

		# optionally, draw a circle where the lines meet
		intersectionX = x1 + (uA * (x2-x1))
		intersectionY = y1 + (uA * (y2-y1))

		cv2.circle(img, (int(intersectionX), int(intersectionY)), 4, (0, 0, 255), -1)

		return True
	return False

def detectCollision(line, detectionLines, img):

	left = False
	right = False
	direction = 0

	#both[Both lines][left or right][which line][coordinate]
	for BothLines in detectionLines:
		for LeftOrRight in BothLines:
			for DetectionLine in LeftOrRight:
				if lineLine(img, line, DetectionLine):
					printLine(img, DetectionLine, (255, 255, 0), 5)
					if(direction == 0):
						left = True
					elif(direction == 1):
						right = True
		direction += 1

	return [left, right]



def lineDetection(lines, bothLines, img):

	font = cv2.FONT_HERSHEY_SIMPLEX
	if lines is not None:
		i = 0
		j = 0
		
		# Loop through lines
		for line in lines:
			(x1,y1,x2,y2) = line[0]

			# Compare every line
			for lineCheck in lines: 

				xC1,yC1,xC2,yC2 = lineCheck[0]
				distance = math.sqrt( (x1-xC1)**2 + (y1-yC1)**2 )

				slopeln = round( float(slope( line[0] )), 0 )
				slopelnC = round( float(slope( lineCheck[0] )), 0 )

				cv2.putText( img, str(slopeln),(x2,y2), font, 0.6,(255,255,255),2,cv2.LINE_AA )
				cv2.putText( img, str(slopelnC),(xC2, yC2), font, 0.6,(255,255,255),2,cv2.LINE_AA )

				# Check that line isn't the same line
				if i != j: 
					
					# Check if line is parallel
					if isParallel( line[0], lineCheck[0], 10 ): 

						# Check if line is close enough
						if(distance > 3 and distance <= 100):

							printLine(img, line[0], (0, 0, 255)) # Parallel line
							printLine(img, lineCheck[0], (0, 0, 255)) # Parallel line

							# Check if line collides with any of the detection lines
							lineDetect = detectCollision(list(line[0]), bothLines, img)
							print(str(lineDetect))
							if( lineDetect[0] and lineDetect[1] ):
								return "front"
							elif (lineDetect[0]):
								printLine(img, line[0], (0, 255, 0)) # Parallel line
								printLine(img, lineCheck[0], (0, 255, 0)) # Parallel line
								return "left"
							elif (lineDetect[1]):
								printLine(img, line[0], (255, 0, 0)) # Parallel line
								printLine(img, lineCheck[0], (255, 0, 0)) # Parallel line
								return "right"
							else:
								return "No tape"


				j += 1
			i += 1

def printLine(img, (x1, y1, x2, y2), (b, g, r), thickness = 3):
	cv2.line(img, (x1,y1), (x2,y2), (b, g, r), thickness) # Parallel line


def BlackTape(frame, follow):
	# resolution
	try:
		from camera_pi import Camera_pi
		w, h = Camera_pi.getSettings()
	except:
		from camera_opencv import Camera_opencv
		w, h = Camera_opencv.getSettings()

	leftLines = []
	rightLines = []
	bothLines = []


	if (follow):
		rx, ry = w / 6, h / 2
		rw, rh = w / 6 * 4, h / 2

		leftLines.append([rx, ry, rx, ry+rh])
		rightLines.append([rx+rw, ry, rx+rw, ry+rh])
		
	else:
		x1, y1 = int(0), int(h / 5 * 4)
		x2, y2 = int(w / 5 * 2), int(h / 5 * 4)

		x3, y3 = int(w / 5 * 3), int(h / 5 * 4)
		x4, y4 = int(w), int(h / 5 * 4)

		leftLines.append([x3, y3, x4, y4])
		rightLines.append([x1, y1, x2, y2])

	bothLines.append([ leftLines, rightLines ])

	#img = cv2.bilateralFilter(img,9,75,75)
	edges = cv2.Canny(frame, 255,411,apertureSize = 3)
	lines = cv2.HoughLinesP(edges, 1, np.pi/180, 100, maxLineGap=35)
	return lineDetection(lines, bothLines, frame)
