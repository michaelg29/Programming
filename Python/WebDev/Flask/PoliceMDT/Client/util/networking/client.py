import socket
import threading

class Client:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def sendMsg(self):
        while True:
            self.sock.send(bytes(input('').encode()))

    def __init__(self):
        pass

    def bind(address, port):
        self.sock.connect((address, port))

        iThread = threading.Thread(target=self.sendMsg)
        iThread.daemon = True # close when app ends
        iThread.start() # in background

        while True:
            data = self.sock.recv(1024)
            if not data:
                break   # disconnect
            print(data.decode())
