from flask import Flask, session, redirect, url_for, request, render_template, Blueprint
from datetime import datetime
from controllers.unauthorized import unauthorized_pages
import actions

app = Flask(__name__)
app.secret_key = 'secret_key'
app.register_blueprint(unauthorized_pages)

@app.before_first_request
def start():
    session['user_id']         = -1
    session['message_present'] = False
    actions.start()

@app.context_processor
def utility_processor():
    def showMessage():
        session['message_present'] = False
        return ''
    def getURL(action):
        return url_for(action)
    def formatDate(date):
        return str(datetime(date['year'], date['month'], date['day'], date['hour'], date['minute']))
    def getAlphabet():
        return 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    def log(item):
        print(item)
        return ''
    return dict(showMessage = showMessage, getURL = getURL, formatDate = formatDate, getAlphabet = getAlphabet, log = log)

@app.route('/')
def index():
    return render_template('index.html')

if __name__ == '__main__':
    app.run('127.0.0.1', 5000)