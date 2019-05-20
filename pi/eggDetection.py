import sys
import cv2
import numpy as np
import time
from blobDetectionPi import DetectEgg

debug = False
if len(sys.argv) > 1:
	if sys.argv[1] == '-d':
		debug = True

egg = DetectEgg(debug).DetectEgg()

if egg:
	print "Found an egg!"
else:
	print "No egg to see."
