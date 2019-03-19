from library.WebServer import WebServer

def home(request):
    if "name" in request.params.keys():
        request.client.context["name"] = request.params["name"]
    request.render_template("index.html")

if __name__ == "__main__":
    ws = WebServer("127.0.0.1", 8080)

    ws.routes = { 
        "/" : home,
    }

    ws.atts.contextRoute = "content"
    ws.atts.errorFile = "error.html"

    ws.run()