from .TcpListener import TcpListener
from .WebClient import WebClient
from .Request import Request

class WebServer(TcpListener):
    def __init__(self, ipAddr, port):
        super().__init__(ipAddr, port)
        self.routes = {}
        self.clients = []

    def clientConnected(self, clientSock, clientAddr):
        client = WebClient(clientSock, clientAddr)
        self.clients.append(client)

    def clientDisconnected(self, clientSock, clientAddr):
        pass

    def msgReceived(self, clientSock, clientAddr, msg):
        for client in self.clients:
            if client.sock == clientSock:
                request = Request(msg, client)
                request.parse()

                print(request.method + " " + request.route)

                for key in self.routes.keys():
                    if key == request.route:
                        self.routes[key](request)

                self.send(clientSock, request.getResponse())

                break

    def serverEvent(self, msg):
        print("SERVER>", msg)

    def open(self):
        super().open()

    def stop(self):
        super().stop()