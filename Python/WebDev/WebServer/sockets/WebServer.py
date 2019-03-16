from .TcpListener import TcpListener

class WebServer(TcpListener):
    def __init__(self, ipAddr, port):
        super().__init__(ipAddr, port)

    def clientConnected(self, clientSock, clientAddr):
        print("client connected at ", clientAddr)

    def clientDisconnected(self, clientSock, clientAddr):
        print("client disconnected at ", clientAddr)

    def msgReceived(self, clientSock, clientAddr, msg):
        print("client at", clientAddr, "said:\n\t", msg)

    def serverEvent(self, msg):
        print("SERVER>", msg)

    def open(self):
        super().open()

    def stop(self):
        super().stop()