import cv2
import numpy as np
import sys
import time


#get from socket
from client import SocketReceive
SocketReceive()
Qualify = SocketReceive()

socket
import socket
sok = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sok.connect((socket.gethostname(), 1234))

def SocketReceive():
    msg_send = "Ask"
    utf8_msg = unicode(msg_send, "utf-8")
     output
    sok.send(bytes(utf8_msg))
    rec = sok.recv(1024, 0)
    return rec

def SocketSend(msg):
    utf8_msg = unicode(msg, "utf-8")

sok.send(bytes(utf8_msg))

#SWITCH CASE QUALIFY OR RACE
def switchQualify(Qualify):
    switcher = {
        #QUALIFIERS
        #gate
        GateQualify:Poort(),
        #grit path
        GritQualify:Grindpad(),
        #gripper
        GripperQualify:Gripper(),
        #vision
        VisionQualify:BlauwBalk(),
        #RACES
        #chicken got talent
        DanceRace:ChickenGotTalent(),
        #eggtelligence
        EggRace:Eggtelligence(),
        #chicken stairs and slope
        SlopeRace:ChickenTrap(),
        #capture the flag     
        FlagRace:Flag()
    }

#KWALIFICATIES AUTONOOM
#2 POORT QUALIFY
def Poort():
    #go through gate #doing
    from gateRun import Gate
    Gate()

#3 GRINDPAD QUALIFY
def Grindpad():
    # go over grit path #todo
    from gritRun import Grit
    Grit()

#4 GRIPPER QUALIFY
def Gripper:
    #grab and hold fresh egg #todo
    from eggGrab import Grab
    Grab()

#5 VISION QUALIFY
def BlauwBalk:
    #detect and follow blue beam #doing
    from blueBeam import ViewBeam
    ViewBeam()
    

#RACES, AUTONOMOUS STAIRS AND EGGTELLIGENCE, REMAINDER REMOTE

#1 CHICKEN RACE, speed in a straight line

#2 CHICKEN GOT TALENT, dancing on music, touch line once

#3 CHICKEN BOOGIE LINE DANCE, dancing on music in line

#4 CHICKEN TRAP RACE
def ChickenTrap:
    #stairs detection #todo
    from stairs import UpStairs
    UpStairs()

#5 EGGTELLIGENCE RACE
def Eggtelligence:
    DetectPhase ="default"
    #BLACK TAPE    
    #black tape #doing
    from tape import BlackTape
    BlackTape()

    #PATH FINDING
    #pathfinder algorithm #todo
    #from pathfinding import FindPath
    #FindPath()

    #EI OPPAKKEN EN BRENGEN NAAR BAKJE MET QR
    if DetectPhase =="default":
        #egg detection #done
        from blobDetection import EggDetection
        EggDetection()
    elif DetectPhase =="eggDetected":
        #go near #todo
        print("GoToEgg")
    elif DetectPhase =="eggNear":
        #egg grabbing #todo
        print("GrabEgg")
    elif DetectPhase =="eggGrabbed":
        #chicken detection #todo
        from chickenRun import ChickenDetection
        ChickenDetection()
    elif DetectPhase =="trayFind":
        #trays detection #doing
        from distance import main
        main()
    elif DetectPhase =="trayNear":
        #QR detection #done
        from qrvid import ShowQR
        ShowQR()
    else:
        #do nothing



#6 CAPTURE THE FLAG RACE, remote control


















