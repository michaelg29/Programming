class App:
    routes = {}
    name = ""

    def getRoutes(self):
        ret = {}

        if self.name:
            for key in self.routes.keys():
                ret[self.name + "/" + key] = self.routes[key]
        
        else:
            ret.update(self.routes)

        return ret