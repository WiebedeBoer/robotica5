import picamera
import picamera.array


def getPiCamera():
    with picamera.PiCamera() as camera:
        camera.resolution = (1280, 720)
        with picamera.array.PiRGBArray(camera, size=(640, 360)) as output:
            frame = camera.capture(output, 'rgb', resize=(640, 360))
            return frame

        