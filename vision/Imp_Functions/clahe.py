import cv2

def CLAHE(frame):
	clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
	cl = clahe.apply(frame)

	return cl