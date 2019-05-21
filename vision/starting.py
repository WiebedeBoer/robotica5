import sys
import socket


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
def mainSwitcher(argument, argument1, argument2):
    switcher = {
        0: kwalificatie,
        1: wedstrijd,
    }

    func = switcher.get(argument, "Nothing")
    if(func is kwalificatie):
        return func(argument1)
    else:
        return func(argument1, argument2)


# Kwalificatie switcher
def kwalificatie(argument):
    def kwalificatieSwitcher(argument):
        switcher = {
            0: pitch,
            1: poortje,
            2: grindpad,
            3: eiGripper,
            4: vision
        }
        func = switcher.get(argument, "Nothing")
        return func()

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
            0: chickenSurvivalRun,
            1: eggtelligence
        }
        func = switcher.get(argument, "Nothing")
        if (func is chickenSurvivalRun):
            return func()
        else:
            return func(argument1)


    def chickenSurvivalRun():
        sys.path.append('Wedstrijd/ChickenSurvivalRun/')
        test = False

    def eggtelligence(argument):
        def eggtelligenceSwitcher(argument):
            switcher = {
                0: eggDistance,
                1: qrDistance
            }
            func = switcher.get(argument, "Nothing")
            return func()

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


sok = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sok.connect((socket.gethostname(), 1234))

while True:
    msg = SocketReceive()
    msg = splitter(msg)

    msgBack =  mainSwitcher(int(msg[0]), int(msg[1]), int(msg[2]))
    SocketSend(msgBack)
