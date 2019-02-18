from django.http import HttpResponse
from django.shortcuts import render
from . import context_processors

def home(request):
    return render(request, 'MundusInvicte/home.html')

def about(request):
    return render(request, 'MundusInvicte/about.html')