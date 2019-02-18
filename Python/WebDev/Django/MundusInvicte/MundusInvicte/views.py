from django.http import HttpResponse
from django.shortcuts import render
from . import context_processors

def home(request):
    context_processors.editContext('loggedIn', True)
    return render(request, 'MundusInvicte/home.html')

def about(request):
    context_processors.editContext('loggedIn', False)
    return render(request, 'MundusInvicte/about.html')