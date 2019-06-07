import sys
import cv2
import numpy as np
import clahe as cl
import math

img = None
def callback(x):
	pass

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

		if(slopeX > 0 and abs(slopeX) < 100):
			return slopeX
		elif (slopeY > 0 and abs(slopeY) < 100):
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

	if slope1 == "NAN" or slope2 == "NAN":
		return False


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

		cv2.circle(img, (intersectionX, intersectionY), 4, (0, 0, 255), -1)

		return True
	return False

def detectCollision(line, detectionLines, img):

	left = False
	right = False

	#both[Both lines][left or right][which line][coordinate]
	for i in range(0, len(detectionLines)):
		for j in range(0, len(detectionLines[i])):
			for k in range(0, len(detectionLines[i][j])):
				if lineLine(img, line, detectionLines[i][j][k]):
					printLine(img, detectionLines[i][j][k], (255, 255, 0), 5)
					print(detectionLines[i][j][k])
					if(j == 0):
						left = True
					elif(j == 1):
						right = True
					printLine(img, detectionLines[i][j][k], (255,0,0))

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
					if isParallel( line[0], lineCheck[0], 411 ): 

						# Check if line is close enough
						if(distance > 3 and distance <= 100):

							printLine(img, line[0], (0, 0, 255)) # Parallel line
							printLine(img, lineCheck[0], (0, 0, 255)) # Parallel line

							# Check if line collides with any of the detection lines
							print(line[0])
							print(bothLines[0][0][0][0])
							lineDetect = lineLine(line[0], bothLines[0][0][0], img)
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


def BlackTape(follow):

	# resolution
	w = 640
	h = 640

	cam = cv2.VideoCapture(0)
	cam.set(cv2.CAP_PROP_FRAME_WIDTH, w)
	cam.set(cv2.CAP_PROP_FRAME_HEIGHT, h)
	#cam.set(w, h)

	x, y = 0, 0

	cv2.namedWindow('settings', 2)

	cv2.createTrackbar('H_MIN','settings',0,255,callback)
	cv2.createTrackbar('S_MIN','settings',0,255,callback)
	cv2.createTrackbar('V_MIN','settings',0,255,callback)

	cv2.createTrackbar('H_MAX','settings',0,255,callback)
	cv2.createTrackbar('S_MAX','settings',0,255,callback)
	cv2.createTrackbar('V_MAX','settings',0,255,callback)

	cv2.createTrackbar('TH_MIN','settings',0,500,callback)
	cv2.createTrackbar('TH_MAX','settings',0,500,callback)

	cv2.createTrackbar('RHO','settings',1,20,callback)
	cv2.createTrackbar('TH','settings',2,50,callback)

	cv2.createTrackbar('MIN_LENGTH','settings',0,500,callback)
	cv2.createTrackbar('MAX_GAP','settings',0,500,callback)

	cv2.createTrackbar('MAX_DISTANCE','settings',100,500,callback)
	cv2.createTrackbar('MIN_DISTANCE','settings',3,500,callback)

	cv2.createTrackbar('iterations','settings',0,100,callback)


	cv2.setTrackbarPos('H_MAX','settings',255)
	cv2.setTrackbarPos('S_MAX','settings',255)
	cv2.setTrackbarPos('V_MAX','settings',255)

	cv2.setTrackbarPos('TH_MIN','settings',255)
	cv2.setTrackbarPos('TH_MAX','settings',411)

	cv2.setTrackbarPos('MIN_LENGTH','settings',70)
	cv2.setTrackbarPos('MAX_GAP','settings',35)

	while(cam.isOpened()):


		#[30,227,18],[105,94,84]

		h_min = cv2.getTrackbarPos('H_MIN','settings')
		s_min = cv2.getTrackbarPos('S_MIN','settings')
		v_min = cv2.getTrackbarPos('V_MIN','settings')

		h_max = cv2.getTrackbarPos('H_MAX','settings')
		s_max = cv2.getTrackbarPos('S_MAX','settings')
		v_max = cv2.getTrackbarPos('V_MAX','settings')

		th_min = cv2.getTrackbarPos('TH_MIN','settings')
		th_max = cv2.getTrackbarPos('TH_MAX','settings')

		min_length = cv2.getTrackbarPos('MIN_LENGTH','settings')
		max_gap = cv2.getTrackbarPos('MAX_GAP','settings')
		
		max_dist = cv2.getTrackbarPos('MAX_DISTANCE','settings')
		min_dist = cv2.getTrackbarPos('MIN_DISTANCE','settings')

		iterations = cv2.getTrackbarPos('iterations','settings')

		rho = cv2.getTrackbarPos('RHO','settings')
		th_slope = cv2.getTrackbarPos('TH','settings')

		lower_tape = np.array([h_min,s_min,v_min])
		upper_tape = np.array([h_max,s_max,v_max])

		_, img = cam.read()

		#cv2.imshow("frame", img)

		# THIS LINE CAN BE REMOVED FOR THE PI #
		img = removeBlackBars(img)
		# THIS LINE CAN BE REMOVED FOR THE PI #

		(windowHeight, windowWidth, _) = img.shape

		img = cv2.bilateralFilter(img,9,75,75)
		gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

		th = cv2.inRange(img, lower_tape, upper_tape)
		tape = cv2.bitwise_and(img, img, mask=th)
		edges = cv2.Canny(tape,th_min,th_max,apertureSize = 3)

		lines = cv2.HoughLinesP(edges, rho, np.pi/180, min_length, maxLineGap=max_gap)

		leftLines = []
		rightLines = []
		bothLines = []

		if (follow):
			rx, ry = windowWidth / 6, windowHeight / 2
			rw, rh = windowWidth / 6 * 4, windowHeight / 2

			leftLines.append([rx, ry, rx, ry+rh])
			rightLines.append([rx+rw, ry, rx+rw, ry+rh])

			# both[All lines][left or right][which line][coordinate]
			bothLines.append([ leftLines, rightLines ])

			print( lineDetection(lines, bothLines, img) )

		else:
			x1, y1 = 0, windowHeight / 5 * 4
			x2, y2 = windowWidth / 5 * 2, windowHeight / 5 * 4

			x3, y3 = windowWidth / 5 * 3, windowHeight / 5 * 4
			x4, y4 = windowWidth, windowHeight / 5 * 4

			printLine(img, (x1, y1, x2, y2), (0, 255, 0), 1)
			printLine(img, (x3, y3, x4, y4), (0, 255, 0), 1)

			leftLines.append([x3, y3, x4, y4])
			rightLines.append([x1, y1, x2, y2])
			
			# both[All lines][left or right][which line][coordinate]
			bothLines.append([ leftLines, rightLines ])

			print( lineDetection(lines, bothLines, img) )
		

		cv2.imshow("tape", tape)
		cv2.imshow("th", th)
		cv2.imshow("edges", edges)
		cv2.imshow("default", img)


		if cv2.waitKey(1) & 0xFF == ord('q'):
			break

	cam.release()
	cv2.destroyAllWindows()
	
BlackTape(True)