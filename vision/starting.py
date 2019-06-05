import sys
import socket
# from camera_opencv import getPiCamera
import os
import cv2

#os.chdir(os.path.realpath(__file__+ '\\..\\')) # If on windows use this
os.chdir(os.path.realpath(__file__+ '//..//'))  # If on liunx use this

# Here the function gets the string send by the pi
def SocketReceive():
    rec = sok.recv(1024, 0)
    return rec


# Here the function sends a string ba ck to the pi
def SocketSend(msg):
    utf8_msg = unicode(str(msg), "utf-8")
    sok.send(bytes(utf8_msg))


def splitter(msg):
    return msg.split(':')


# Main switcher
def mainSwitcher(frame, argument, argument1, argument2):
    switcher = {
        0: pitch,
        1: vision,
        2: chickenSurvivalRun,
        3: eggtelligence
    }

    func = switcher.get(argument, "Nothing")
    return func(frame, argument1, argument2)


def pitch(frame, argument, argument1):
    return "Hello"


def vision(frame, argument, argument1):
    sys.path.append('Kwalificatie/Vision/')
    from blueBeam import viewBeam
    return viewBeam(frame)


def chickenSurvivalRun(frame, argument, argument1):
    sys.path.append('Wedstrijd/ChickenSurvivalRun/')
    test = False
    return test


def eggtelligence(frame, argument, argument1):
    sys.path.append('Wedstrijd/Eggtelligence/')
    from eggtelligence import eggtelligence
    return eggtelligence(frame, argument, argument1)


cap = cv2.VideoCapture(0)
try:
    while True:
        # frame = getCapture()
        _, frame = cap.read()
        print(mainSwitcher(frame, 3, 1, 0))
        # cv2.imshow("frame", frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

except KeyboardInterrupt:
    cap.release()
    cv2.destroyAllWindows()

# sok = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# try:
#     sok.connect((socket.gethostname(), 1234))
#
#     while True:
#     	frame = getPiCamera()
#         msg = SocketReceive()
#         msg = splitter(msg)
#
#         msgBack = mainSwitcher(frame, int(msg[0]), int(msg[1]), int(msg[2]))
#         SocketSend(msgBack)
#
# except Exception, e:
#     sok.close()
#     print e
#
# finally:
#     sok.close()