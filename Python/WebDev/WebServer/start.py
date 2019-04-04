from library.WebServer import WebServer
from apps.home.home import HomeApp

def home(request):
    if "name" in request.params.keys():
        request.client.context["name"] = request.params["name"]
    request.render_template("index.html")

def dosomething():
    print("Context method")

IP_ADDRESS = "127.0.0.1"
PORT = 8080

if __name__ == "__main__":
    # instantiate server
    ws = WebServer(IP_ADDRESS, PORT)

    # set properties
    ws.atts.contextRoute = "content"
    ws.atts.errorFile = "error.html"

    # set routes
    ws.routes = { 
        "" : home,
    }

    # add apps
    ws.routes.update(HomeApp().getRoutes())

    # add context methods
    context_methods = {
        "print" : dosomething
    }
    ws.registerContextMethods(context_methods)

    ws.run()