from flask import Flask, session, redirect, url_for, request, render_template, Blueprint

def setMessage(content, type):
    session['message'] = content
    session['message_type'] = type
    session['message_present'] = True

def getURL(action):
    return url_for(action)