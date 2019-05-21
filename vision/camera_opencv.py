from picamera.array import PiRGBArray
from picamera import PiCamera

# Camera init
camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 60
rawCapture = PiRGBArray(camera, size=(640, 480))

def getPiCamera():

    for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=1):
        frame = frame.array
        return frame