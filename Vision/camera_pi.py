from picamera.array import PiRGBArray
from picamera import PiCamera


class Camera_pi:
    __instance = None
    __width = 1280
    __height = 720

    @staticmethod
    def getSettings():
        """ Static access method. """
        if Camera_pi.__instance == None:
            Camera_pi.__instance = PiCamera()
            Camera_pi.__instance.resolution = (Camera_pi.__width, Camera_pi.__height)
            Camera_pi.__instance.framerate = 30
        return int(Camera_pi.__width), int(Camera_pi.__height)


    @staticmethod
    def getInstance():
        """ Static access method. """
        if Camera_pi.__instance == None:
            Camera_pi.__instance = PiCamera()
            Camera_pi.__instance.resolution = (Camera_pi.__width, Camera_pi.__height)
            Camera_pi.__instance.framerate = 30

        rawCapture = PiRGBArray(Camera_pi.__instance, size=(Camera_pi.__width, Camera_pi.__height))
        for frame in Camera_pi.__instance.capture_continuous(rawCapture, format="bgr", use_video_port=1):
            rawCapture.truncate()
            rawCapture.seek(0)
            frame = frame.array
            return frame

    def __init__(self):
        """ Virtually private constructor. """
        if Camera_pi.__instance != None:
            raise Exception("This class is a singleton!")
        else:
            Camera_pi.__instance = self
