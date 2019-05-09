import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("127.0.0.1", 1234))


s.send(bytes("Hey there!!!"))
s.send(bytes("msg2k"))

while True:
    input1 = raw_input()
    u = unicode(input1, "utf-8")
    # output
    print(u)
    s.send(bytes(u))
    if input1 == "exit":
        break
s.close()
