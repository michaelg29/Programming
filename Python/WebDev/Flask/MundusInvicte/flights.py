from flask import Flask, session, redirect, url_for, request, render_template, Blueprint
from util import setMessage, getURL
import json

flight_pages = Blueprint('flight_pages', __name__, template_folder='templates')

@flight_pages.route('/flights')
def flights():
    flights = json.load(open('data/flights.json'))['flights']

    return render_template('flights.html', flights = flights)

@flight_pages.route('/flight/<id>')
def flight(id):
    flights = json.load(open('data/flights.json'))
    
    for flight in flights['flights']:
        if flight['id'] == id:
            return render_template('flight.html', flight = flight)

    setMessage('Unable to find flight with that identification tag.', 'error')
    return redirect(getURL('flights'))