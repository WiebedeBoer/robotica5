from picamera.array import PiRGBArray
from picamera import PiCamera

# Camera init
camera = PiCamera()
camera.resolution = (1280, 720)
camera.framerate = 30
rawCapture = PiRGBArray(camera, size=(1280, 720))

def getPiCamera():
    for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=1):
        frame = frame.array
        return frame