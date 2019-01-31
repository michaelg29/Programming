from flask import Flask, redirect, url_for, request, render_template

clicks = 0

app = Flask(__name__)

@app.route('/')
def index():
    global clicks
    clicks += 1
    return "Welcome to the app, %d." % clicks

@app.route('/hello/<name>/<int:id>') #app.add_url_rule('/hello', 'hello', hello)
def hello(name, id):
    return "Hello, %s! Your id is %d" % (name, id)

@app.route('/admin')
def hello_admin():
    global clicks
    clicks += 1
    return 'Hello admin %d' % clicks

@app.route('/guest/<guest>/<int:id>')
def hello_guest(guest, id):
    values = {'purchases':50,'money':1020}
    return render_template("hello.html", name = guest, id = id, atts = values)

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
        return redirect(url_for('user', name = user))
    else:
        return render_template("login.html")

if __name__ == '__main__':
    app.run('127.0.0.1', 5000)