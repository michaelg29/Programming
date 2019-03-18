from flask import Flask, session, redirect, url_for, request, render_template, Blueprint
from .util import setMessage, getURL
import json

user_pages = Blueprint('user_pages', __name__, template_folder='templates')

@user_pages.route('/login', methods=['POST', 'GET'])
def login():
    if request.method == 'POST':
        data = request.form
        user = data['username']
        password = data['password']

        accounts = json.load(open('data/accounts.json'))

        for person in accounts['users']:
            if person['username'] == user:
                if person['password'] == password:
                    session['user_id'] = person['id']
                    setMessage('You successfully logged in, hello %s' % user, 'success')
                    return redirect(getURL('index'))
            
        setMessage('Invalid username or password', 'error')
        return render_template("user/login.html")
    else:
        if session['user_id'] != -1:
            return redirect(getURL('settings'))
        else:
            return render_template("user/login.html")

@user_pages.route('/logout')
def logout():
    session['user_id'] = -1
    setMessage('You successfully logged out', 'success')
    return redirect(getURL('index'))