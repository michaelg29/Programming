import socket, threading

class TcpListener:
    def __init__(self, ipAddr, port):
        self.__ipAddr = ipAddr
        self.__port = port
        self.__server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.__running = False

    # override this
    def clientConnected(self, clientSock, clientAddr):
        pass

    # override this
    def clientDisconnected(self, clientSock, clientAddr):
        pass

    # override this
    def msgReceived(self, clientSock, clientAddr, msg):
        pass

    # override this
    def serverEvent(self, msg):
        pass

    # don't overrride
    def acceptThread(self):
        self.__running = True

        while self.__running:
            self.__server.listen(1)
            clientSock, clientAddr = self.__server.accept()
            thread = threading.Thread(target=self.clientThread, args=(clientSock, clientAddr))
            thread.start()

    # don't override
    def clientThread(self, clientSock, clientAddr):
        self.clientConnected(clientSock, clientAddr)
        msg = ''
        while True:
            data = clientSock.recv(2048)
            msg = data.decode()
            if not msg:
                break
            elif msg.isspace():
                continue
            self.msgReceived(clientSock, clientAddr, msg)
            self.send(clientSock, msg)
        clientSock.close()
        self.clientDisconnected(clientSock, clientAddr)

    # don't override
    def send(self, clientSock, msg):
        clientSock.send(bytes(msg,'UTF-8'))

    def stop(self):
        self.__running = False
        self.__incomingConnectionThread.join()
        self.serverEvent("Server will close when all clients disconnected")
        self.__server.close()

    def open(self):
        self.__server.bind((self.__ipAddr, self.__port))

        self.serverEvent("Server started at " + self.__ipAddr + ":" + str(self.__port))
        
        self.__incomingConnectionThread = threading.Thread(target=self.acceptThread)
        self.__incomingConnectionThread.start()
        