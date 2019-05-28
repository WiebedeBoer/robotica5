import sys
import cv2
import numpy as np
import clahe as cl
import math

def callback(x):
	pass

def slope(var):
	return (var[2] - var[0]) / (var[3] - var[1])

def isParallel(var1, var2, threshold):

	slope1 = slope(var1)
	slope2 = slope(var2)

	print "slope1 = " + str(slope1)
	print "slope2 = " + str(slope2)

	if slope1 == slope2:
		return True
	elif slope1 > slope2 and slope1 <= slope2 + threshold:
		print "approx higher"
		return True
	elif slope1 < slope2 and slope1 >= slope2 - threshold:
		print "approx lower"
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
	

def BlackTape():

	# resolution
	w = 640
	h = 480

	cam = cv2.VideoCapture(0) 
	cam.set(w,h)

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
	cv2.createTrackbar('TH','settings',0,50,callback)

	cv2.createTrackbar('MIN_LENGTH','settings',0,500,callback)
	cv2.createTrackbar('MAX_GAP','settings',0,500,callback)

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

		while (True):

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

			iterations = cv2.getTrackbarPos('iterations','settings')

			rho = cv2.getTrackbarPos('RHO','settings')
			th_slope = cv2.getTrackbarPos('TH','settings')

			lower_tape = np.array([h_min,s_min,v_min])
			upper_tape = np.array([h_max,s_max,v_max])

			_, img = cam.read()

			# THIS LINE CAN BE REMOVED FOR THE PI #
			img = removeBlackBars(img)
			# THIS LINE CAN BE REMOVED FOR THE PI #

			img = cv2.bilateralFilter(img,9,75,75)
			
			gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

			th = cv2.inRange(img, lower_tape, upper_tape)

			tape = cv2.bitwise_and(img, img, mask=th)
			edges = cv2.Canny(tape,th_min,th_max,apertureSize = 3)

			lines = cv2.HoughLinesP(edges, rho, np.pi/180, min_length, maxLineGap=max_gap)

			font = cv2.FONT_HERSHEY_SIMPLEX
			if lines is not None:
				i = 0
				j = 0
				
				for line in lines:
					x1,y1,x2,y2 = line[0]
					cv2.line(img, (x1,y1), (x2,y2), (0, 0, 255), 1) # all lines
					line[0] = map(lambda x: x - w/2, line[0])
					
					for lineCheck in lines: # check every line if parallel

						xC1,yC1,xC2,yC2 = lineCheck[0]
						distance = math.sqrt( (x1-xC1)**2 + (y1-yC1)**2 )
						cv2.putText(img, str( slope(line[0]) ),(x1,y1), font, 0.7,(255,255,255),2,cv2.LINE_AA)
						cv2.putText(img, str( slope(lineCheck[0]) ),(xC1,yC1), font, 0.7,(255,255,255),2,cv2.LINE_AA)

						if(distance < 20):
							#cv2.line(img, (x1,y1), (xC1,yC1), (255, 0, 0), 2) # close lines

							
							

							#if line[0].all() != lineCheck[0].all():
							#if x1 != xC1 and y1 != yC1 and x2 != xC2 and y2 != yC2:
							#if True:
							if i != j:
								

								if isParallel( line[0], lineCheck[0], th_slope ):
									print "gucci"
									cv2.line(img, (x1,y1), (x2,y2), (0, 255, 0), 3) # Parallel line
									cv2.line(img, (x1,y1), (xC1,yC1), (255, 255, 0), 1) # Parallel line connection
								else:
									print "not gucci"
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
