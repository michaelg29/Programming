context = {}

def home(request):
    return context

def editContext(key, value):
    global context
    context[key] = value