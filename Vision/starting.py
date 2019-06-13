import sys
import socket
import os
import cv2

try:
	# from camera_pi import Camera_pi
	from picamera.array import PiRGBArray
	from picamera import PiCamera
except:
	from camera_opencv import Camera_opencv

# os.chdir(os.path.realpath(__file__+ '\\..\\')) # If on windows use this
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
		0: tape,
		1: vision,
		2: chickenSurvivalRun,
		3: eggtelligence
	}

	func = switcher.get(argument, "Nothing")
	return func(frame, argument1, argument2)


def tape(frame, argument, argument1):
	sys.path.append('Imp_Functions/')
	from tapetest import BlackTape
	return BlackTape(frame, False if argument == 0 else True )


def vision(frame, argument, argument1):
	sys.path.append('Kwalificatie/Vision/')
	from blueBeam import viewBeam
	return viewBeam(frame)

def chickenSurvivalRun(frame, argument, argument1):
	sys.path.append('Wedstrijd/ChickenSurvivalRun/')
	from chickenSurvivalRun import chickenSurvivalRun
	return chickenSurvivalRun(frame)


def eggtelligence(frame, argument, argument1):
	sys.path.append('Wedstrijd/Eggtelligence/')
	from eggtelligence import eggtelligence
	return eggtelligence(frame, argument, argument1)


def debug(arg):
	if(arg == "-p"):
		camera = PiCamera()
		camera.resolution = (640, 480)
		camera.framerate = 32
		rawCapture = PiRGBArray(camera, size=(640, 480))

		while True:
			for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
				img = frame.array
				print(mainSwitcher(img, 0, 0, 0))

	if(arg == "-o"):
		cap = Camera_opencv.getInstance()
		try:
			while True:
				_, frame = cap.read()
				print(mainSwitcher(frame, 0, 0, 0))
				cv2.imshow('frame', frame)
				if cv2.waitKey(1) & 0xFF == ord('q'):
					break
		except KeyboardInterrupt:
			cap.release()
			cv2.destroyAllWindows()

def release():
	try:
		global sok
		sok = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		sok.connect((socket.gethostname(), 1234))
		while True:
			frame = Camera_pi.getInstance()
			msg = SocketReceive()
			msg = splitter(msg)
			msgBack = mainSwitcher(frame, int(msg[0]), int(msg[1]), int(msg[2]))
			SocketSend(msgBack)
	except Exception, e:
		sok.close()
		print e
	finally:
		 sok.close()


if(len(sys.argv) > 1 and len(sys.argv) < 4):
	if(sys.argv[1] == '-d'):
		if (len(sys.argv) == 3):
			debug(sys.argv[2])
		else:
			print("please gimme more args pls...")
	elif(sys.argv[1] == '-r'):
		release()
	else:
		print("something went wrong.")
else:
	print("no args given")
