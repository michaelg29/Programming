from .TcpListener import TcpListener
from .WebClient import WebClient
from .Request import Request
from jinja2 import Environment, FileSystemLoader

class WebServerAttributes:
    def __init__(self, contextRoute, errorFile):
        self.contextRoute = contextRoute
        self.errorFile = errorFile
        self.jinja_env = Environment(loader=FileSystemLoader(self.contextRoute))

class WebServer(TcpListener):
    def __init__(self, ipAddr, port):
        super().__init__(ipAddr, port, True)
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

                if request.type != "text/css":
                    found = False

                    for key in self.routes.keys():
                        if key == request.route[1:]:
                            self.routes[key](request)
                            found = True

                    if not found:
                        request.render_template(self.atts.errorFile)

                self.send(clientSock, request.getResponse())

                break

    def serverEvent(self, msg):
        print("SERVER>", msg)

    def cmdThread(self):
        cmd = input()
        if cmd == "stop":
            print("Server shutting down")
            exit()

    def run(self):
        super().open()
        print("Type 'stop' to stop server")
