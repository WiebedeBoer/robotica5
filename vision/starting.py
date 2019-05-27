import sys
import socket
import cv2
from camera_opencv import getPiCamera
import numpy

# Here the function gets the string send by the pi
def SocketReceive():
    rec = sok.recv(1024, 0)
    return rec


# Here the function sends a string back to the pi
def SocketSend(msg):
    utf8_msg = unicode(str(msg), "utf-8")
    sok.send(bytes(utf8_msg))


def splitter(msg):
    return msg.split(':')


# Main switcher
def mainSwitcher(argument, argument1, argument2, frame):
    switcher = {
        0: pitch,
        1: vision,
        2: chickenSurvivalRun,
        3: eggtelligence
    }

    func = switcher.get(argument, "Nothing")
    return func(argument1, argument2, frame)


def pitch(argument, argument1, frame):
    return "Hello"


def vision(argument, argument1, frame):
    sys.path.append('Kwalificatie/Vision/')
    from blueBeam import viewBeam
    return viewBeam(frame)


def chickenSurvivalRun(argument, argument1, frame):
    sys.path.append('Wedstrijd/ChickenSurvivalRun/')
    test = False


def eggtelligence(argument, argument1, frame):
    def eggtelligenceSwitcher(argument, argument1, frame):
        switcher = {
            0: eggDistance,
            1: qrDistance
        }
        func = switcher.get(argument, "Something went wrong")
        return func(argument1, frame)

    def eggDistance(argument, frame):
        sys.path.append('Wedstrijd/Eggtelligence/')
        from startEggDistance import startEggDistance
        return startEggDistance(frame)

    def qrDistance(argument, frame):
        sys.path.append('Wedstrijd/Eggtelligence/')
        from startQRDistance import startQRDistance
        return startQRDistance(townSwitcher(argument), frame)
        
    def townSwitcher(argument):
        switcher = {
            0: "'s-Hertogenbosch",
            1: "Eindhoven",
            2: "Eibergen",
            3: "Barneveld",
            4: "Duckstad"
        }
        func = switcher.get(argument, "Something went wrong")
        return func

    eggtelligenceSwitcher(argument, argument1, frame)



# cap = cv2.VideoCapture(0)
# try:
#     while True:
#         # frame = getCapture()
#         _, frame = cap.read()
#         print(mainSwitcher(3, 1, 0, frame))
#         # cv2.imshow("frame", frame)
#         if cv2.waitKey(1) & 0xFF == ord('q'):
#             break
#
# except KeyboardInterrupt:
#     cap.release()
#     cv2.destroyAllWindows()

sok = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
    sok.connect((socket.gethostname(), 1234))

    while True:
    	frame = getPiCamera()
        msg = SocketReceive()
        msg = splitter(msg)

        msgBack = mainSwitcher(int(msg[0]), int(msg[1]), int(msg[2]), frame)
        #SocketSend(msgBack)

except Exception, e:
    sok.close()
    print e

finally:
    sok.close()
