from library.WebServer import WebServer
from apps.home.home import HomeApp

def home(request):
    if "name" in request.params.keys():
        request.client.context["name"] = request.params["name"]
    request.render_template("index.html")

if __name__ == "__main__":
    ws = WebServer("127.0.0.1", 8080)

    ws.routes = { 
        "" : home,
    }

    ws.routes.update(HomeApp().getRoutes())

    ws.atts.contextRoute = "content"
    ws.atts.errorFile = "error.html"

    ws.run()