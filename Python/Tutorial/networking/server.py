import socket
import sys
from _thread import *
from threading import Thread

class TcpServer:
    HOST = '0.0.0.0'
    PORT = 9999
    CLIENTS = []

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    RUNNING = False

    def __init__(self, host, port):
        self.HOST = host
        self.PORT = port

    def logger(self, msg):
        print(msg)

    def start(self):
        try:
            self.s.bind((self.HOST, self.PORT))
        except socket.error:
            self.logger("binding failed")
            sys.exit()

        self.logger("socket created and bound")

        self.s.listen(10)
        self.logger("socket ready")
        self.RUNNING = True

        while True:
            cmd = input()
            if cmd == 'exit':
                for conn in self.CLIENTS:
                    conn.send("Goodbye".encode())
                    conn.close()
                self.RUNNING = False
                break

        sys.exit()

        self.s.close()
        self.logger("Disconnected")

    def clientthread(self, conn):
        conn.send("Welcome to the server. Type something in:\n\t".encode())
        self.CLIENTS.append(conn)

        while True:
            data = conn.recv(1024)
            reply = "OK." + data.decode()
            if not data: # broken connection
                break
            self.logger(reply)
            conn.sendall(reply.encode())

        conn.close()

    def waitthread(self):
        while self.RUNNING:
            conn, addr = self.s.accept()
            self.logger("Connected with " + addr[0] + ":" + str(addr[1]))
            start_new_thread(clientthread, (conn,))

serv = TcpServer('192.168.1.193', 9999)
serv.start()