import cv2
import numpy as np
import sys
import time

#EI OPPAKKEN EN BRENGEN NAAR BAKJE MET QR

#ei detection
from egg import ShowCamFeed
ShowCamFeed()

#QR detection
from qrvid import ShowQR
ShowQR()

#bakjes detection
from test_all_colors import Bakje
Bakje()

#kip detection
from chicken_run import Kip
Kip()

#POORT KWALIFICATIE
from poort_run import Poort
Poort()

#PATH FINDING

#pathfinder algoritme
from pathfinding import FindPath
FindPath()

#HELLING RACE

#trap detection
from trap import TrapOp
TrapOp()
