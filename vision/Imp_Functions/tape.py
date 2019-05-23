import sys
import cv2
import numpy as np
import clahe as cl

def removeGlare(hsv, iterations, th):

	# glare
	lower = th
	upper = np.array([255,255,255])

	# Threshhold on the lightness
	glare_th = cv2.inRange(hsv, lower, upper)

	# Dilate glare
	kernel = np.ones((5,5),np.uint8)
	glare_th = cv2.morphologyEx(glare_th, cv2.MORPH_CLOSE, kernel)

	# Cut out glare
	glare = cv2.bitwise_and(hsv, hsv, mask=glare_th)
	no_glare = cv2.bitwise_xor(hsv, glare)

	# Correct glare and paste it back
	glare[:,:,2] = glare[:,:,2] *0.5
	hsv = no_glare + glare


	lower[2] = lower[2] * 0.95
	if iterations > 0:
		iterations -= 1
		return removeGlare(hsv, iterations, lower)
	else:
		return hsv
	

def BlackTape():
	cam = cv2.VideoCapture(0)
	#cam = cv2.VideoCapture('tape.avi')    

	while(cam.isOpened()):
		lower_tape = np.array([0,0,0])
		upper_tape = np.array([255,255,100])
		#[30,227,18],[105,94,84]

		while (True):
			_, img = cam.read()
			
			hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
			newimg = removeGlare(hsv, 5, np.array([0,0,220]))
			tape = cv2.inRange(corrected, lower_tape, upper_tape)
			corrected = cv2.cvtColor(newimg, cv2.COLOR_HSV2BGR)


			

			#cv2.imshow("corrected", corrected)
			cv2.imshow("tape", tape)
			cv2.imshow("img", img)
			cv2.imshow("newimg", corrected)


			if cv2.waitKey(1) & 0xFF == ord('q'):
				break

		cam.release()
		cv2.destroyAllWindows()
	
BlackTape()
