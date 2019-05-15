import socket
sok = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sok.connect((socket.gethostname(), 1234))


def SocketReceive():
    msg_send = "Ask"
    utf8_msg = unicode(msg_send, "utf-8")
    # output
    sok.send(bytes(utf8_msg))
    rec = sok.recv(1024, 0)
    print rec


