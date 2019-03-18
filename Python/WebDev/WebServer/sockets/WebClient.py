import socket
import os

class WebClient:
    def __init__(self, socket, addr):
        self.sock = socket
        self.addr = addr
        self.context = {}