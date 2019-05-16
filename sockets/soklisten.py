import socket

def SocketReceive():
    # output
    rec = sok.recv(1024, 0)
    return rec


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
    if cmd == "Ask":
        print SocketReceive()
        cmd = raw_input()

        SocketSend(cmd)

