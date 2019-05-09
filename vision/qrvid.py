import numpy as np
import cv2
import pyzbar.pyzbar as pyzbar

def ShowQR():
	cap = cv2.VideoCapture(0)
	while(1):
		_, frame = cap.read()
		
		cv2.imshow('frame', frame)
		decodedObjects = pyzbar.decode(frame)
		for obj in decodedObjects:
			print("Type:", obj.type)
			print("Data: ", obj.data, "\n")

		cv2.imshow("Frame", frame)


		if cv2.waitKey(1) & 0xFF == ord('q'):
			break
	cap.release()
	cv2.destroyAllWindows()

ShowQR()
