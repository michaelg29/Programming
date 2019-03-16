from sockets.WebServer import WebServer
import threading

def cmdThread(ws):
    cmd = input()
    if cmd == "stop":
        ws.stop()

ws = WebServer("127.0.0.1", 9000)

t = threading.Thread(target=cmdThread, args=(ws,))
t.start()

ws.open()

t.join()