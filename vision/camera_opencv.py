from picamera.array import PiRGBArray
from picamera import PiCamera

# Camera init
camera = PiCamera()
camera.resolution = (1280, 720)
camera.framerate = 30

def getPiCamera():
    rawCapture = PiRGBArray(camera, size=(1280, 720))
    for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=1):
        rawCapture.truncate()
        rawCapture.seek(0)
        frame = frame.array
        return frame