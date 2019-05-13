import io
import time
import cv2
from picamera.array import PiRGBArray
from picamera import PiCamera

camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 30
rawCapture = PiRGBArray(camera, size=(640,480))

time.sleep(0.1)

def cameraPreview():
	for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=1):
		image = frame.array
		cv2.imshow("Frame", image)
		
		k = cv2.waitKey(1) & 0xFF
		if k == 27:
			break
		rawCapture.truncate(0)

	cv2.destroyAllWindows()

cameraPreview()
