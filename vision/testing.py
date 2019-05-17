
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
def mainSwitcher(argument, argument1):
    switcher = {
        0: kwalificatie(argument1),
        1: wedstrijd(),
    }
    return switcher.get(argument, "Nothing")

def kwalificatie(argument):
    def kwalificatieswitcher(argument):
        switcher = {
            0: pitch(),
            1: egg(),
        }
        return switcher.get(argument, "Nothing")

    def pitch():
        return str("This is a pitch")

    def egg():
        return str("This is a egg")


    return kwalificatieswitcher(argument)



def wedstrijd():
    return str("hello from wedstrijd")


print(mainSwitcher(0, 1))