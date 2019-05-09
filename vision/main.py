import cv2
import numpy as np
import sys
import time

#EI OPPAKKEN EN BRENGEN NAAR BAKJE MET QR

#ei detection #done
from blobDetection import EggDetection
EggDetection()

#QR detection #done
from qrvid import ShowQR
ShowQR()

#black tape #doing
from tape import BlackTape
BlackTape()

#bakjes detection #doing
from distance import main
main()

#kip detection #todo
from chickenRun import ChickenDetection
ChickenDetection()

#POORT KWALIFICATIE

#poort detection #todo
from poortRun import Poort
Poort()

#PATH FINDING

#pathfinder algoritme #todo
from pathfinding import FindPath
FindPath()

#HELLING RACE

#trap detection #todo
from stairs import UpStairs
UpStairs()
