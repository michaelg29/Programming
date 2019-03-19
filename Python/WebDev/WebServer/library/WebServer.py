from .TcpListener import TcpListener
from .WebClient import WebClient
from .Request import Request

class WebServerAttributes:
    def __init__(self, contextRoute, errorFile):
        self.contextRoute = contextRoute
        self.errorFile = errorFile

class WebServer(TcpListener):
    def __init__(self, ipAddr, port):
        super().__init__(ipAddr, port)
        self.routes = {}
        self.clients = []
        self.atts = WebServerAttributes("content", "error.html")

    def clientConnected(self, clientSock, clientAddr):
        client = WebClient(clientSock, clientAddr)
        self.clients.append(client)

    def clientDisconnected(self, clientSock, clientAddr):
        pass

    def msgReceived(self, clientSock, clientAddr, msg):
        for client in self.clients:
            if client.sock == clientSock:
                request = Request(msg, client, self.atts)
                request.parse()

                print(request.method + " " + request.route)

                found = False

                for key in self.routes.keys():
                    if key == request.route:
                        self.routes[key](request)
                        found = True

                if not found:
                    request.render_template(self.atts.errorFile)

                self.send(clientSock, request.getResponse())

                break

    def serverEvent(self, msg):
        print("SERVER>", msg)

    def open(self):
        super().open()

    def stop(self):
        super().stop()