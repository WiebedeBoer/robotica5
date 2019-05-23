import sys
import cv2
import numpy as np
import clahe as cl

def callback():
	pass

def removeBlackBars(img):
	gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
	_,th = cv2.threshold(gray,1,255,cv2.THRESH_BINARY)

	contours,hierarchy = cv2.findContours(th,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
	cnt = contours[0]
	x,y,w,h = cv2.boundingRect(cnt)

	crop = img[y:y+h,x:x+w]
	return crop

def removeGlare(hsv, iterations, th):

	# glare
	lower = th
	upper = np.array([255,255,255])

	# Threshhold on the lightness
	glare_th = cv2.inRange(hsv, lower, upper)

	# Dilate glare
	kernel = np.ones((10,10),np.uint8)
	glare_th = cv2.morphologyEx(glare_th, cv2.MORPH_CLOSE, kernel)

	# Cut out glare
	glare = cv2.bitwise_and(hsv, hsv, mask=glare_th)
	no_glare = cv2.bitwise_xor(hsv, glare)

	# Correct glare and paste it back
	glare[:,:,2] = glare[:,:,2] *0.8
	hsv = no_glare + glare


	lower[2] = lower[2] *0.9
	if iterations > 0:
		iterations -= 1
		return removeGlare(hsv, iterations, lower)
	else:
		return hsv

def removeGlareHSV(hsv, th_min, th_max):

	hsv = cv2.bitwise_not(hsv)
	ret,th = cv2.threshold(hsv[:,:,1],th_min,th_max,cv2.THRESH_BINARY)
	cv2.imshow("th", th)

	# Dilate glare
	kernel = np.ones((10,10),np.uint8)
	glare_th = cv2.morphologyEx(th, cv2.MORPH_CLOSE, kernel)

	# Cut out glare
	glare = cv2.bitwise_and(hsv, hsv, mask=glare_th)
	no_glare = cv2.bitwise_xor(hsv, glare)

	# Correct glare and paste it back
	glare[:,:,2] = glare[:,:,2] * 0.6
	res = no_glare + glare
	res = cv2.cvtColor(res, cv2.COLOR_HSV2BGR)

	cv2.imshow("glare", glare)
	cv2.imshow("no_glare", no_glare)
	cv2.imshow("hsv_input", hsv)
	cv2.imshow("res", res)

	return res
	

def BlackTape():
	cam = cv2.VideoCapture(0)  

	cv2.namedWindow('settings', 2)

	cv2.createTrackbar('H_MIN','settings',0,255,callback)
	cv2.createTrackbar('S_MIN','settings',0,255,callback)
	cv2.createTrackbar('V_MIN','settings',0,255,callback)

	cv2.createTrackbar('H_MAX','settings',0,255,callback)
	cv2.createTrackbar('S_MAX','settings',0,255,callback)
	cv2.createTrackbar('V_MAX','settings',0,255,callback)

	cv2.createTrackbar('TH_MIN','settings',0,255,callback)
	cv2.createTrackbar('TH_MAX','settings',0,255,callback)

	cv2.createTrackbar('iterations','settings',0,100,callback)

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

			iterations = cv2.getTrackbarPos('iterations','settings')

			lower_tape = np.array([h_min,s_min,v_min])
			upper_tape = np.array([h_max,s_max,v_max])

			_, img = cam.read()
			img = removeBlackBars(img)
			img = cv2.GaussianBlur(img, (5, 5), 0)
			gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

			_,th = cv2.threshold(gray,70,255,cv2.THRESH_BINARY)

			cut = cv2.bitwise_and(img, img, mask=th)
			tape = cv2.bitwise_xor(img, cut)		
			edges = cv2.Canny(tape,50,150,apertureSize = 3)

			lines = cv2.HoughLinesP(edges, 1, np.pi/180, 100, maxLineGap=250)
			if lines is not None:
				for line in lines:
					x1, y1, x2, y2 = line[0]
					cv2.line(img, (x1, y1), (x2, y2), (0, 255, 0), 5)
			
			#hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
			#newimg = hsv
			#newimg = removeGlare(hsv, iterations, np.array([0,0,250]))
			#newhsv = removeGlareHSV(hsv, th_min, th_max)

			#tape = cv2.inRange(newimg, lower_tape, upper_tape)

			#kernel = np.ones((5,5),np.uint8)
			#tape = cv2.morphologyEx(tape, cv2.MORPH_CLOSE, kernel)

			#corrected = cv2.cvtColor(newimg, cv2.COLOR_HSV2BGR)

			# cv2.imshow("tape", tape)
			cv2.imshow("tape", tape)
			cv2.imshow("th", th)
			cv2.imshow("edges", edges)
			cv2.imshow("single_channel", img)


			if cv2.waitKey(1) & 0xFF == ord('q'):
				break

		cam.release()
		cv2.destroyAllWindows()
	
BlackTape()
