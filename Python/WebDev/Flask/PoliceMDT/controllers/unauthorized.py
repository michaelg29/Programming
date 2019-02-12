from flask import Flask, session, redirect, url_for, request, render_template, Blueprint
from util.database import dbController
from actions import getURL, executeSQL

unauthorized_pages = Blueprint('unauthorized_pages', __name__, template_folder='templates/unauthorized')

@unauthorized_pages.route('/login', methods=['POST', 'GET'])
def login():
    if request.method == 'POST':
        data = request.form
        user = data['username']
        password = data['password']

        results = executeSQL('select * from dbo.mdt_Users where Username = \'%s\' and Password = \'%s\'' % (user, password))
        if len(results) > 0:
            return redirect(url_for('index'))
            
        setMessage('Invalid username or password', 'error')
        return render_template("login.html")
    else:
        if session['user_id'] != -1:
            return redirect(url_for('settings'))
        else:
            return render_template("login.html")