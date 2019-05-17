
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
def mainSwitcher(argument, arg1):
    switcher = {
        0: kwalificatie,
        1: wedstrijd,
    }
    func = switcher.get(argument, "Nothing")
    return func(arg1)

def kwalificatie(arg1):
    def kwalificatieswitcher(arg1):
        switcher = {
            0: pitch,
            1: egg,
        }
        func = switcher.get(arg1, "Nothing")
        return func()

    def pitch():
        print("Dikke boktor")
        return str("This is a pitch")

    def egg():
        print("Dikke shit")
        return str("This is a egg")


    return kwalificatieswitcher(arg1)



def wedstrijd():
    return str("hello from wedstrijd")


print(mainSwitcher(1))