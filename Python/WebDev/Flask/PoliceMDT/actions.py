from flask import Flask, session, redirect, url_for, request, render_template, Blueprint
#from util.networking import client
#from util.database import dbController

def start():
    # TODO: make readonly account
    #dbController.connect('127.0.0.1', 'MundusInvicte', 'sa', 'SO4--sulfate')

    # TODO: connect to server with client
    pass

def setMessage(content, type):
    session['message'] = content
    session['message_type'] = type
    session['message_present'] = True

def getURL(action):
    return url_for(action)

def validateUsername(deptId, username, password):
    return True