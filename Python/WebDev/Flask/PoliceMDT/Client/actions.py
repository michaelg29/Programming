from flask import Flask, session, redirect, url_for, request, render_template, Blueprint
#from util.networking import client
from util.database import dbController

import pyodbc

cnxn = pyodbc.Connection
cursor = pyodbc.Cursor

def start():
    global cnxn, cursor
    cnxn = dbController.connect('localhost\sqlexpress', 'MundusInvicte', 'client', 'client123')
    cursor = cnxn.cursor()

    # TODO: connect to server with client
    pass

def setMessage(content, type):
    session['message'] = content
    session['message_type'] = type
    session['message_present'] = True

def executeSQL(query):
    return dbController.execute(cursor, query)