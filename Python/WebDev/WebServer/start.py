from sockets.WebServer import WebServer
from sockets.Request import Request
import threading

def home(request):
    request.client.context["name"] = request.params["name"]
    request.render_template("content/index.html")

def cmdThread(ws):
    cmd = input()
    if cmd == "stop":
	    ws.stop()

if __name__ == "__main__":
    ws = WebServer("127.0.0.1", 8080)

    ws.routes = { "/" : home, 
                  "/index" : home,
    }

    t = threading.Thread(target=cmdThread, args=(ws,))
    t.start()

    ws.open()

    t.join()

