from picamera.array import PiRGBArray
from picamera import PiCamera

# Camera init
camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 30

def getPiCamera():
    rawCapture = PiRGBArray(camera, size=(640, 480))
    for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=1):
        rawCapture.truncate()
        rawCapture.seek()
        frame = frame.array
        return frame