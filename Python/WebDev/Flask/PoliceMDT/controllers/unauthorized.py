from flask import Flask, session, redirect, url_for, request, render_template, Blueprint

unauthorized_pages = Blueprint('unauthorized_pages', __name__, template_folder='templates/unauthorized')