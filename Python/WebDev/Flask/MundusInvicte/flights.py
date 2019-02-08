from flask import Flask, session, redirect, url_for, request, render_template, Blueprint
from util import setMessage, getURL
import json

flight_pages = Blueprint('flight_pages', __name__, template_folder='templates')

def parseSeats(seat_str):
    seats = []
    rows = seat_str.split(';')
    for row in rows:
        row_seats = row.split(',')
        seats.append(row_seats)
    return seats

@flight_pages.route('/flights')
def flights():
    flights = json.load(open('data/flights.json'))['flights']

    return render_template('flights/flights.html', flights = flights)

@flight_pages.route('/flight/<id>')
def flight(id):
    flights = json.load(open('data/flights.json'))
    
    for flight in flights['flights']:
        if flight['id'] == id:
            return render_template('flights/flight.html', flight = flight, seats = parseSeats(flight['seats']))

    setMessage('Unable to find flight with that identification tag.', 'error')
    return redirect(getURL('flights'))

@flight_pages.route('/flight/<id>/seat/<seat>')
def seat(id, seat):
    flights = json.load(open('data/flights.json'))
    
    for flight in flights['flights']:
        if flight['id'] == id:
            seats = parseSeats(flight['seats'])
            
            return render_template('flights/seat.html', flight = flight, seat = )

    setMessage('Unable to find flight with that identification tag.', 'error')
    return redirect(getURL('flights'))