import sys
import cv2
import numpy as np
import clahe as cl
import math

def callback(x):
	pass

def slope(var):
	x1 = float( var[0] )
	y1 = float( var[1] )
	x2 = float( var[2] )
	y2 = float( var[3] )

	if (x2 - x1) > 0:
		return ( (y2 - y1) / (x2 - x1) ) * 180.0 / np.pi
	elif (y2 - y1) > 0:
		return ( (x2 - x1) / (y2 - y1) ) * 180.0 / np.pi
	else:
		return "NAN"

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

def removeGlare(lab, iterations, th = 0):

	# glare
	lower = th
	upper = np.array([255,255,255])

	# Threshhold on the lightness
	glare_th = cv2.inRange(lab, lower, upper)

	# Dilate glare
	kernel = np.ones((5,5),np.uint8)
	glare_th = cv2.dilate(glare_th,kernel,iterations = 1)

	# Cut out glare
	glare = cv2.bitwise_and(lab, lab, mask=glare_th)
	no_glare = cv2.bitwise_xor(lab, glare)

	# Correct glare and paste it back
	glare[:,:,0] = glare[:,:,0] *0.4
	lab = no_glare + glare

	# lower[0] = lower[0] *0.999

	cv2.imshow("glare_", glare)
	cv2.imshow("no_glare", no_glare)
	cv2.imshow("hsv_input", lab)

	if iterations > 0:
		iterations -= 1
		return removeGlare( lab, iterations, lower )
	else:
		return cv2.cvtColor( lab, cv2.COLOR_Lab2BGR )

def removeGlareHSV(lab, th_min, th_max):

	lab = cv2.bitwise_not(lab)
	ret,th = cv2.threshold(hsv[:,:,1],th_min,th_max,cv2.THRESH_BINARY)

	# Dilate glare
	kernel = np.ones((10,10),np.uint8)
	glare_th = cv2.morphologyEx(th, cv2.MORPH_CLOSE, kernel)

	# Cut out glare
	glare = cv2.bitwise_and(hsv, hsv, mask=glare_th)
	no_glare = cv2.bitwise_xor(hsv, glare)

	# Correct glare and paste it back
	glare[:,:,2] = glare[:,:,2] * 0.9
	res = no_glare + glare
	res = cv2.cvtColor(res, cv2.COLOR_Lab2BGR)

	return res

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
	left =   lineLine(x1,y1,x2,y2, rx,ry,rx, ry+rh, img)
	right =  lineLine(x1,y1,x2,y2, rx+rw,ry, rx+rw,ry+rh, img)
	top =    lineLine(x1,y1,x2,y2, rx,ry, rx+rw,ry, img)
	bottom = lineLine(x1,y1,x2,y2, rx,ry+rh, rx+rw,ry+rh, img)

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
		return True
	return False

def lineLine(x1, y1, x2, y2, x3, y3, x4, y4, img):

	# calculate the direction of the lines
	uA = None
	uB = None
	try:
		uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1))
		uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1))
	except ZeroDivisionError:
		print("division by zero")
		return False

	# if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 and uA <= 1 and uB >= 0 and uB <= 1):

		print("COLLIDE")

		# optionally, draw a circle where the lines meet
		intersectionX = x1 + (uA * (x2-x1))
		intersectionY = y1 + (uA * (y2-y1))

		cv2.circle(img, (int( intersectionX ), int( intersectionY )), 3, (255, 0, 0), 2)

		return True
	return False

	

def BlackTape():

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

	cv2.createTrackbar('MAX_DISTANCE','settings',15,500,callback)
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

		rx, ry = windowWidth / 4, windowHeight / 2
		rw, rh = windowWidth / 2, windowHeight / 2

		cv2.rectangle(img, (rx, ry), (rx + rw, ry + rh), (0, 255, 0))

		font = cv2.FONT_HERSHEY_SIMPLEX

		if lines is not None:
			i = 0
			j = 0
			
			for line in lines: # Loop through lines
				x1,y1,x2,y2 = line[0]

				for lineCheck in lines: # Compare every line

					xC1,yC1,xC2,yC2 = lineCheck[0]
					distance = math.sqrt( (x1-xC1)**2 + (y1-yC1)**2 )

					slopeln = round( float(slope( line[0] )), 0 )
					slopelnC = round( float(slope( lineCheck[0] )), 0 )

					cv2.putText( img, str(slopeln),(x2,y2), font, 0.6,(255,255,255),2,cv2.LINE_AA )
					cv2.putText( img, str(slopelnC),(xC2, yC2), font, 0.6,(255,255,255),2,cv2.LINE_AA )

					if i != j: # Check that line isn't the same line
						
						if isParallel( line[0], lineCheck[0], th_slope ): # Check if line is parallel

							if(distance > min_dist and distance <= max_dist): # Check if line is close enough

								cv2.line(img, (x1,y1), (x2,y2), (0, 0, 255), 1) # Draw parallel lines

								# Check if line intersects with rectangle
								if( lineRect(x1, y1, x2, y2, rx, ry, rw, rh, img) or lineRect(xC1, yC1, xC2, yC2, rx, ry, rw, rh, img) ):
									cv2.line(img, (x1,y1), (x2,y2), (0, 255, 0), 3) # Parallel line
									cv2.line(img, (xC1,yC1), (xC2,yC2), (0, 255, 0), 3) # Parallel line
					j += 1
				i += 1

		cv2.imshow("tape", tape)
		cv2.imshow("th", th)
		cv2.imshow("edges", edges)
		cv2.imshow("default", img)


		if cv2.waitKey(1) & 0xFF == ord('q'):
			break

	cam.release()
	cv2.destroyAllWindows()
	
BlackTape()
