from flask import Flask, session, redirect, url_for, request, render_template, Blueprint
from util.database import dbController
from actions import getURL, executeSQL

unauthorized_pages = Blueprint('unauthorized_pages', __name__, template_folder='templates/unauthorized')

@unauthorized_pages.route('/login', methods=['POST', 'GET'])
def login():
    if request.method == 'POST':
        data = request.form
        dept_code = data['dept_code']
        user = data['username']
        password = data['password']

        results = executeSQL('select * from dbo.mdt_Users where DeptCode = %d' % int(dept_code))
        if len(results) > 0:
            results = executeSQL('select * from dbo.mdt_Users where DeptCode = %d and Username = \'%s\' and Password = \'%s\'' % (int(dept_code), user, password))
            if len(results) > 0:
                session['user_id'] = results[0]
                session['dept_code'] = results[1]
                session['last_name'] = results[2]
                session['first_name'] = results[3]
                session['username'] = results[4]
                return redirect(url_for('index'))
            else:
                setMessage('Invalid username or password', 'error')
        else:
            setMessage('Could not find department code.', 'error')
            
        return render_template("login.html")
    else:
        if session['user_id'] != -1:
            return redirect(url_for('settings'))
        else:
            return render_template("login.html")