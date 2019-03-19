from jinja2 import Template

def parseAttributeString(atts):
    data = {}
    for param in atts.split("&"):
        sep_idx = param.find("=")
        key = param[:sep_idx]
        value = param[sep_idx + 1:]
        data[key] = value

    return data

class Request:
    def __init__(self, request, client, serverAtts):
        self.request_body = request
        self.client = client
        self.method = ""
        self.route = ""
        self.params = {}
        self.data = {}
        self.response_content = ""
        self.response_code = 200
        self.response = ""
        self.type = "text/html"
        self.serverAtts = serverAtts

    def parse(self):
        request = self.request_body.split()

        # request method
        self.method = request[0]

        route = request[1]
        self.route = route

        if self.route.find(".css") == -1:
            query_idx = route.find("?")
            if query_idx != -1:
                # request parameters'
                self.route = route[:query_idx]
                params = route[query_idx + 1:]
                self.params = parseAttributeString(params)

            # form data
            data = request[-1]
            if data.find("=") != -1:
                # has data parameters from form
                self.data = parseAttributeString(data)
        else:
            self.type = "text/css"
            self.render_template(self.route)

    def render_template(self, file_path):
        content = ""

        try:
            with open(self.serverAtts.contextRoute + "/" + file_path, 'r') as content_file:
                content = content_file.read()
        except:
            self.response_code = 404
            with open(self.serverAtts.contextRoute + "/" + self.serverAtts.errorFile, 'r') as error_file:
                content = error_file.read()
                self.render_content(content)
        else:
            if self.route.find(".css") == -1:
                self.render_content(content)
            else:
                self.response_content = content

    def render_content(self, content):
        template = Template(content)
        self.response_content = template.render(self.client.context)

    def getResponse(self):
        return_char = "\r\n"
        self.response = "HTTP/1.1 " + str(self.response_code) + " OK" + return_char + \
                        "Cache-Control: no-cache, private" + return_char + \
                        "Content-Type: " + self.type + return_char + \
                        "Content-Length: " + str(len(self.response_content)) + return_char + \
                        str(return_char) + \
                        self.response_content

        return self.response