import socket
#stopt met alles tot dat er een bericht van de server is ontvangen
def SocketReceive():
    # output
    rec = sok.recv(1024, 0)
    return rec

#verstuur msg naar de server pi
def SocketSend(msg):
    utf8_msg = unicode(msg, "utf-8")
    sok.send(bytes(utf8_msg))


sok = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sok.connect((socket.gethostname(), 1234))

while True:
    #if cmd == "Ask":
    print SocketReceive()
    #else:
    cmd = raw_input()

    SocketSend(cmd)

