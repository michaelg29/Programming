import socket
import threading
import sys

class Server:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    connections = []

    def __init__(self):
        self.sock.bind(('127.0.0.1', 9999))
        self.sock.listen(1)

    def handler(self, c, a):
        while True:
            try:
                data = c.recv(1024)
                print(data)
                for connection in self.connections:
                    connection.send(data)
                if not data:
                    print(str(a[0]) + ':' + str(a[1]), 'disconnected')
                    self.connections.remove(c)
                    c.close()
                    break # disconnect
            except ConnectionResetError:
                print(str(a[0]) + ':' + str(a[1]), 'disconnected')
                self.connections.remove(c)
                c.close()
                break # disconnect

    def run(self):
        while True:
            c, a = self.sock.accept()
            cThread = threading.Thread(target=self.handler, args=(c, a))
            cThread.daemon = True
            cThread.start()
            self.connections.append(c)
            print(str(a[0]) + ':' + str(a[1]), 'connected')

class Client:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def sendMsg(self):
        while True:
            self.sock.send(bytes(input('').encode()))

    def __init__(self, address, port):
        self.sock.connect((address, port))

        iThread = threading.Thread(target=self.sendMsg)
        iThread.daemon = True # close when app ends
        iThread.start() # in background

        while True:
            data = self.sock.recv(1024)
            if not data:
                break   # disconnect
            print(data.decode())

if (len(sys.argv) > 1):
    client = Client(sys.argv[1], int(sys.argv[2]))
else:
    server = Server()
    server.run()