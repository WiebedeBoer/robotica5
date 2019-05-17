import sys
import socket


# Here the function gets the string send by the pi
def SocketReceive():
    rec = sok.recv(1024, 0)
    return rec


# Here the function sends a string back to the pi
def SocketSend(msg):
    utf8_msg = unicode(msg, "utf-8")
    sok.send(bytes(utf8_msg))


def splitter(msg):
    return msg.split(':')


# Main switcher
def mainSwitcher(argument, argument1, argument2):
    switcher = {
        0: kwalificatie(argument1),
        1: wedstrijd(argument1, argument2),
    }
    return switcher.get(argument, "nothing")


# Kwalificatie switcher
def kwalificatie(argument):
    def kwalificatieSwitcher(argument):
        switcher = {
            0: pitch(),
            1: poortje(),
            2: grindpad(),
            3: eiGripper(),
            4: vision()
        }
        return switcher.get(argument, "nothing")

    def pitch():
        sys.path.append('Kwalificatie/Pitch/')

    def poortje():
        sys.path.append('Kwalificatie/Poortje/')
        #from gateRun import gate
        #return gate()

    def grindpad():
        sys.path.append('Kwalificatie/Grindpad/')

    def eiGripper():
        sys.path.append('Kwalificatie/EiGripper/')

    def vision():
        sys.path.append('Kwalificatie/Vision/')
        #from blueBeam import viewBeam
        #return viewBeam()

    return kwalificatieSwitcher(argument)


def wedstrijd(argument, argument1):
    # Wedstrijd switcher
    def wedstrijdSwitcher(argument, argument1):
        switcher = {
            0: chickenSurvivalRun(),
            1: eggtelligence(argument1)
        }
        return switcher.get(argument, "nothing")

    def chickenSurvivalRun():
        sys.path.append('Wedstrijd/ChickenSurvivalRun/')
        test = False

    def eggtelligence(argument):
        def eggtelligenceSwitcher(argument):
            switcher = {
                0: eggDistance(),
                1: qrDistance()
            }
            return switcher.get(argument, "nothing")

        def eggDistance():
            sys.path.append('Wedstrijd/Eggtelligence/')
            from startEggDistance import startEggDistance
            return startEggDistance()

        def qrDistance():
            sys.path.append('Wedstrijd/Eggtelligence/')
            from startQRDistance import startQRDistance
            return startQRDistance("http://'s-Hertogenbosch")

        return eggtelligenceSwitcher(argument)

    return wedstrijdSwitcher(argument, argument1)

print(mainSwitcher(1,1,0))