from library.app import App
from . import views

class HomeApp(App):
    routes = {
        "shop" : views.index
    }

    name = ""