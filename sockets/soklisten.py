import sys
sys.path.append('../vision/')

import socket
from starting import mainSwitcher

# Here the function gets the string send by the pi
# stopt met alles tot dat er een bericht van de server is ontvangen
def SocketReceive():
    rec = sok.recv(1024, 0)
    return rec

# Here the function sends a string back to the pi
def SocketSend(msg):
    utf8_msg = unicode(msg, "utf-8")
    sok.send(bytes(utf8_msg))


def splitter(msg):
    return msg.split(':')


#sok = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#sok.connect((socket.gethostname(), 1234))

while True:
    cmd = "Ask"

    #SocketSend(cmd)
    if cmd == "Ask":
        # getMsg = SocketReceive()
        getMsg = "1:1:0"
        arrayMsg = splitter(getMsg)
        print("My message: ", arrayMsg)
        mainSwitcher(arrayMsg[0], arrayMsg[1], arrayMsg[2])

        #cmd = raw_input()

        #SocketSend(cmd)
        break

sok = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sok.connect((socket.gethostname(), 1234))

while True:
    #if cmd == "Ask":
    print SocketReceive()
    #else:
    cmd = raw_input()

    SocketSend(cmd)
    if cmd == "Ask":
        print SocketReceive()
        cmd = raw_input()

        SocketSend(cmd)
