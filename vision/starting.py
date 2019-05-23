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
def mainSwitcher(argument, argument1, argument2, argument3):
    switcher = {
        0: kwalificatie,
        1: wedstrijd,
    }

    func = switcher.get(argument, "Nothing")
    return func(argument1, argument2, argument3)


# Kwalificatie switcher
def kwalificatie(argument, argument1 = False, argument2 = False):
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
        return "Hello"

    def vision():
        sys.path.append('Kwalificatie/Vision/')
        from blueBeam import viewBeam
        return viewBeam()

    return kwalificatieSwitcher(argument)


def wedstrijd(argument, argument1, argument2):
    # Wedstrijd switcher
    def wedstrijdSwitcher(argument, argument1, argument2):
        switcher = {
            0: chickenSurvivalRun,
            1: eggtelligence
        }
        func = switcher.get(argument, "Something went wrong")
        return func(argument1, argument2)

    def chickenSurvivalRun(argument = False, argument1 = False):
        sys.path.append('Wedstrijd/ChickenSurvivalRun/')
        test = False

    def eggtelligence(argument, argument1):
        def eggtelligenceSwitcher(argument, argument1):
            switcher = {
                0: eggDistance,
                1: qrDistance
            }
            func = switcher.get(argument, "Something went wrong")
            return func(argument1)

        def eggDistance(argument = False):
            sys.path.append('Wedstrijd/Eggtelligence/')
            from startEggDistance import startEggDistance
            return startEggDistance()

        def qrDistance(argument):
            sys.path.append('Wedstrijd/Eggtelligence/')
            from startQRDistance import startQRDistance
            return startQRDistance(townSwitcher(argument))

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

        return eggtelligenceSwitcher(argument, argument1)

    return wedstrijdSwitcher(argument, argument1, argument2)


sok = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
    sok.connect((socket.gethostname(), 1234))

    while True:
        msg = SocketReceive()
        msg = splitter(msg)

        msgBack = mainSwitcher(int(msg[0]), int(msg[1]), int(msg[2]))

except Exception, e:
    sok.close()
    print e
