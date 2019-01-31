from flask import Flask, session, redirect, url_for, request, render_template

'''
https://www.tutorialspoint.com/flask/flask_file_uploading.htm
'''

app = Flask(__name__)
app.secret_key = 'secret_key'

@app.context_processor
def utility_processor():
    def showMessage():
        session['message_present'] = False
        return ''
    return dict(showMessage = showMessage)

def setMessage(content, type):
    session['message'] = content
    session['message_type'] = type
    session['message_present'] = True

@app.route('/')
def index():
    if 'clicks' in session:
        session['clicks'] += 1
    else:
        session['clicks'] = 1

    return render_template("index.html")

@app.route('/hello/<name>/<int:id>') #app.add_url_rule('/hello', 'hello', hello)
def hello(name, id):
    return "Hello, %s! Your id is %d" % (name, id)

@app.route('/admin')
def hello_admin():
    if 'clicks' in session:
        session['clicks'] += 1
    else:
        session['clicks'] = 1
    
    return 'Hello %s %d' % (session['username'], session['clicks'])

@app.route('/guest/<guest>/<int:id>')
def hello_guest(guest, id):
    session['username'] = guest
    values = {'purchases':50,'money':1020}
    return render_template("hello.html", name = guest, id = id, atts = values)

@app.route('/logout')
def logout():
    if 'username' in session:
        session.pop('username')
    return redirect(url_for('index'))

@app.route('/user/<name>/<int:id>')
def user(name, id):
    if name == 'admin':
        return redirect(url_for('hello_admin'))
    else:
        return redirect(url_for('hello_guest', guest = name, id = id))

@app.route('/login', methods=['POST', 'GET'])
def login():
    if request.method == 'POST':
        user = request.form['nm']
        if user == 'error':
            setMessage('Invalid username', 'error')
            return redirect(url_for('login'))
        else:
            session['username'] = user

        return redirect(url_for('index'))
    else:
        return render_template("login.html")

@app.route('/enter')
def enter():
    return render_template("enterdata.html")

@app.route('/results', methods=['POST', 'GET'])
def results():
    if request.method == 'POST':
        result = request.form
        return render_template("data.html", data = result)

if __name__ == '__main__':
    app.run('127.0.0.1', 5000)