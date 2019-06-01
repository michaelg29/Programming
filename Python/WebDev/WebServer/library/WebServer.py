"""
from .TcpListener import TcpListener
"""

import sys
sys.path.append(r"C:\src\business-library\python")

from mundusinvicte.networking.sockets.TcpListener import TcpListener

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
        self.clients = []
        self.atts = WebServerAttributes("content", "error.html")
        self.routes = {}

    def serverStarted(self):
        print("Server started at", self.ipAddr, "on port", self.port)
        print(r"Type 'stop' to quit")

    def clientConnected(self, client):
        client = WebClient(client.sock, client.addr)
        self.clients.append(client)

    def clientDisconnected(self, client):
        pass

    def msgReceived(self, client, msg):
        request = Request(msg, self.getWebClient(client), self.atts)
        request.parse()

        print(request.method + " " + request.route)

        if request.route.find(".ico") > -1:
            return
        elif request.type == "text/html":
            found = False

            for key in self.routes.keys():
                if key == request.route[1:]:
                    self.routes[key](request)
                    found = True

            if not found:
                request.render_template(self.atts.errorFile)

        client.send(request.getResponse(), False if request.bytes else True)

    def getWebClient(self, client):
        for wclient in self.clients:
            if client.sock == wclient.sock:
                return wclient

        return None

    def registerContextMethods(self, args):
        self.atts.jinja_env.globals.update(args)

    def serverEvent(self, msg):
        print("SERVER>", msg)

    def cmdThread(self):
        cmd = input()
        if cmd == "stop":
            print("Server shutting down")
            exit()
