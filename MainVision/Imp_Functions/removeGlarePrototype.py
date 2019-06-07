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