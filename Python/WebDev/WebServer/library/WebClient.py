import sys
sys.path.append(r"C:\src\business-library\python")

from mundusinvicte.networking.sockets.TcpClient import TcpClient

import socket
import os

class WebClient(TcpClient):
    def __init__(self, socket, addr):
        self.sock = socket
        self.addr = addr
        self.context = {}