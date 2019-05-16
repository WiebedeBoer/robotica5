import sys

# Kwalificatie switcher
def mainSwitcher(argument, value):
    switcher = {
        0: kwalificatie(value),
        1: wedstrijd(value),
    }
    return switcher.get(argument, "nothing")

def kwalificatie(argument):
    # Kwalificatie switcher
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
        from gateRun import gate
        gate()

    def grindpad():
        sys.path.append('Kwalificatie/Grindpad/')

    def eiGripper():
        sys.path.append('Kwalificatie/EiGripper/')

    def vision():
        sys.path.append('Kwalificatie/Vision/')
        from blueBeam import viewBeam
        viewBeam()

    kwalificatieSwitcher(argument)


def wedstrijd(argument):
    # Wedstrijd switcher
    def wedstrijdSwitcher(argument):
        switcher = {
            0: chickenSurvivalRun(),
            1: eggtelligence()
        }
        return switcher.get(argument, "nothing")

    def chickenSurvivalRun():
        sys.path.append('Wedstrijd/ChickenSurvivalRun/')
        test = False

    def eggtelligence():
        sys.path.append('Wedstrijd/Eggtelligence/')
        test = False

    wedstrijdSwitcher(argument)



mainSwitcher(0, 0)