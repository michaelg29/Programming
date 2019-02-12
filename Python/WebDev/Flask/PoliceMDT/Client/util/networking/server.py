import socket
import threading

class Server:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    connections = []

    def __init__(self, address, port):
        self.sock.bind((address, int(port)))
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