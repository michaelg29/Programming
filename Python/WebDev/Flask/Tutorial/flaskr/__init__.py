from flask import Flask, redirect, url_for, request, render_template

app = Flask(__name__)

@app.route('/')
def index():
    return "Welcome to the app."

@app.route('/hello/<name>/<int:id>') #app.add_url_rule('/hello', 'hello', hello)
def hello(name, id):
    return "Hello, %s! Your id is %d" % (name, id)

@app.route('/admin')
def hello_admin():
    return 'Hello admin'

@app.route('/guest/<guest>')
def hello_guest(guest):
    return render_template("hello.html", name = guest)

@app.route('/user/<name>')
def user(name):
    if name == 'admin':
        return redirect(url_for('hello_admin'))
    else:
        return redirect(url_for('hello_guest', guest = name))

@app.route('/login', methods=['POST', 'GET'])
def login():
    if request.method == 'POST':
        user = request.form['nm']
        return redirect(url_for('user', name = user))
    else:
        return render_template("login.html")

if __name__ == '__main__':
    app.run('127.0.0.1', 5000)