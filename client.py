import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((socket.gethostname(), 1234))


s.send(bytes("Hey there!!!", "utf-8"))
s.send(bytes("msg2", "utf-8"))

while True:
    input1 = input()

    # output
    print(input1)
    s.send(bytes(input1, "utf-8"))
    if input1 == "exit":
        break
