import cv2
import numpy as np
import sys
import time

Qualify ="Egg"

#SWITCH CASE QUALIFY OR RACE
def switchQualify(Qualify):
    switcher = {
        #QUALIFIERS
        #poort
        GateQualify:Poort(),
        #grindpad
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
        #chicken trap
        SlopeRace:ChickenTrap(),
        #capture the flag     
        FlagRace:Flag()
    }

#KWALIFICATIES AUTONOOM
#2 POORT KWALIFICATIE
def Poort:
    #poort doorheen gaan #todo
    from gateRun import Gate
    Gate()

#3 GRINDPAD KWALIFICATIE
def Grindpad:
    #over grindpad gaan #todo
    from gritRun import Grit
    Grit()

#4 GRIPPER KWALIFICATIE
def Gripper:
    #vers ei grijpen en vastpakken #todo
    from eggGrab import Grab
    Grab()

#5 VISION KWALIFICATIE
def BlauwBalk:
    #blauw houten balkje detection en volgen #todo
    from blueBeam import ViewBeam
    ViewBeam()
    

#RACES, AUTONOOM TRAP EN EGGTELLIGENCE, REST AFSTANDSBDIENING

#1 CHICKEN RACE, snelheid in rechte lijn

#2 CHICKEN GOT TALENT, dansen op muziek, een keer rand raken

#3 CHICKEN BOOGIE LINE DANCE, dansen op muziek in lijn

#4 CHICKEN TRAP RACE
def ChickenTrap:
    #trap detection #todo
    from stairs import UpStairs
    UpStairs()

#5 EGGTELLIGENCE RACE
def Eggtelligence:
    DetectPhase ="default"
    #TAPE    
    #black tape #doing
    from tape import BlackTape
    BlackTape()

    #PATH FINDING
    #pathfinder algoritme #todo
    #from pathfinding import FindPath
    #FindPath()

    #EI OPPAKKEN EN BRENGEN NAAR BAKJE MET QR
    if DetectPhase =="default":
        #ei detection #done
        from blobDetection import EggDetection
        EggDetection()
    elif DetectPhase =="eggDetected":
        #go near #todo
    elif DetectPhase =="eggNear":
        #egg grabbing #todo
    elif DetectPhase =="eggGrabbed":
        #kip detection #todo
        from chickenRun import ChickenDetection
        ChickenDetection()
    elif DetectPhase =="trayFind":
        #bakjes detection #doing
        from distance import main
        main()
    elif DetectPhase =="trayNear":
        #QR detection #done
        from qrvid import ShowQR
        ShowQR()
    else:
        #do nothing



#6 CAPTURE THE FLAG RACE, afstandsbediening


















