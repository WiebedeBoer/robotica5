import cv2


class Camera_opencv:
    __instance = None
    __width = 640
    __height = 480

    @staticmethod
    def getSettings():
        """ Static access method. """
        if Camera_opencv.__instance == None:
            Camera_opencv.__instance = cv2.VideoCapture(0)
            Camera_opencv.__instance.set(3, Camera_opencv.__width)
            Camera_opencv.__instance.set(4, Camera_opencv.__height)
        return int(Camera_opencv.__width), int(Camera_opencv.__height)


    @staticmethod
    def getInstance():
        """ Static access method. """
        if Camera_opencv.__instance == None:
            Camera_opencv.__instance = cv2.VideoCapture(0)
            Camera_opencv.__instance.set(3, Camera_opencv.__width)
            Camera_opencv.__instance.set(4, Camera_opencv.__height)
        return Camera_opencv.__instance

    def __init__(self):
        """ Virtually private constructor. """
        if Camera_opencv.__instance != None:
            raise Exception("This class is a singleton!")
        else:
            Camera_opencv.__instance = self
