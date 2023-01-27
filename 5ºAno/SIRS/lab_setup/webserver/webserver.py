import flask
import flask_login
import os
import hashlib
import mysql.connector
import hmac
import math
import time

from cryptography.fernet import Fernet
from flask import Flask, render_template, request, redirect, url_for, session, flash
from datetime import timedelta

app = Flask(__name__)
app.config['SECRET_KEY'] = b'{\xf7>c\xc2s\xb3\x0b\x13\xa6=\x01\xf5/\xd8>'
app.config['PERMANENT_SESSION_LIFETIME'] =  timedelta(minutes=3)

#DATABASE INFO
database = mysql.connector.connect(
  host="192.168.2.4",
  user="musicMarkt",
  password="dees",
  database="MusicMarkt",
  ssl_ca="./certificates_keys/ca.pem",
  ssl_cert="./certificates_keys/webserver-cert.pem",
  ssl_key="./certificates_keys/webserver-key.pem"
)

#HASHING/ENCRYPTION
def hash_password(password, salt):
  return hashlib.pbkdf2_hmac(
    'sha256', # The hash digest algorithm for HMAC
    password.encode(), # Convert the password to bytes
    salt, # Provide the salt
    1000 # It is recommended to use at least 100,000 iterations of SHA-256 
  )
SESSION_ENCRYPTION_KEY = b'A1wexjO817pzGgkiR_tW7S4fPmRq3gYL3rjVGy68JXU='
session_cipher = Fernet(SESSION_ENCRYPTION_KEY)

def generate_totp(shared_key: str, length: int = 6) -> str:
    now_in_seconds = math.floor(time.time())
    interval = 20
    t = math.floor(now_in_seconds / interval)
    hash = hmac.new(
        bytes(shared_key, encoding="utf-8"),
        t.to_bytes(length=8, byteorder="big"),
        hashlib.sha256,
    )

    return dynamic_truncation(hash, length)

def dynamic_truncation(raw_key: hmac.HMAC, length: int) -> str:
    bitstring = bin(int(raw_key.hexdigest(), base=16))
    last_four_bits = bitstring[-4:]
    offset = int(last_four_bits, base=2)
    chosen_32_bits = bitstring[offset * 8 : offset * 8 + 32]
    full_totp = str(int(chosen_32_bits, base=2))
    return full_totp[-length:]

def validate_totp(totp: str, shared_key: str) -> bool:
    return totp == generate_totp(shared_key)

#FLASK REQUESTS
@app.before_request
def before_request():
  if session.get('locked') is not None:
    return render_template('locked.html')
  session.modified = True
  flask.g.user = flask_login.current_user
  
## Runs the function once the root page is requested.
## The request comes with the folder structure setting ~/web as the root
@app.route('/')
def index():
  if session.get('user') is not None:
    if session.get('authenticating') is None:
      return render_template("index.html")
    else:
      print("RIGHT HERE")
      session.clear()
  return redirect(url_for('login'))

@app.route('/login', methods=['GET', 'POST'])
def login():
  if request.method == 'POST':
    username, password = request.form['username'], request.form['password']  

    try:
      cursor = database.cursor()
      query = "SELECT * FROM UserInfo WHERE UserName = %s"
      params = (username,)

      cursor.execute(query, params)
    except Exception as e:
      print(e)
      flash("An error as occurred", "error")
      return redirect(url_for('index'))
  
    result = cursor.fetchone()
    print(result)
    if result is not None and hash_password(password, result[3]) == result[1]:
      #IDEA: ADD LIMIT TO ATTEMPTS
      session['user'] = result[0]
      session['authenticating'] = True
      return redirect(url_for("login_2fa"))
    else:
      flash("Incorrect credentials, please try again", "error")
      session['num_attempts'] += 1
      if session['num_attempts'] == 5:
        session['locked'] = True
        return render_template("locked.html")
      
      return render_template("login.html")
    
  else:
    if session.get('user') is not None:
      return redirect(url_for('index'))
    session['num_attempts'] = 0  
    return render_template("login.html")

@app.route("/login/2fa/", methods=['GET','POST'])
def login_2fa():
  if request.method == 'POST' and session.get('authenticating') is not None:
    pin_code = request.form['pin']
    
    #check if pin was correct
    if validate_totp(pin_code, 'e8fb1a2faf331bfffe8670ca20447fae'):
      session.pop('authenticating')
      app.config['PERMANENT_SESSION_LIFETIME'] =  timedelta(minutes=3)
      return redirect(url_for('index'))
    else: #check if too many attempts where made
      session['num_attempts'] += 1
      if session['num_attempts'] == 5:
        session['locked'] = True
        return render_template("locked.html")
      
      flash('Incorrect PIN value, please try again')
      return render_template("login_2fa.html")
  else:
    if session.get('user') is not None:
      if session.get('authenticating') is not None:
        session['num_attempts'] = 0
        #change session lifetime to 1 min for the key code
        app.config['PERMANENT_SESSION_LIFETIME'] =  timedelta(minutes=1)
        session.pop('_flashes', None)
        return render_template("login_2fa.html")
      else:
        return redirect(url_for('index'))
    else:
      return redirect(url_for('login'))

@app.route('/create_account', methods=['GET', 'POST'])
def create_account():
  if request.method == 'POST':
    username, password, phone_number = request.form['username'], request.form['password'], request.form['phone_nr']
    if not username or not password or not phone_number:
      flash("Please fill all the necessary information", "error")
      
    if len(username) > 20 or len(password) > 50 or len(phone_number) != 9:
      flash("Invalid input sizes, phone number must have 9 characters. Please try again")
      return redirect(url_for('create_account'))
    
    try:
      salt = os.urandom(32)
      query = "INSERT INTO UserInfo VALUES (%s, %s, %s, %s)"
      params = (username, hash_password(password, salt), phone_number, salt)
      cursor = database.cursor()
      cursor.execute(query, params)
      database.commit()
    except mysql.connector.IntegrityError:
      flash("Username or phone number already exist", "error")
      return redirect(url_for('index'))

    flash("Account Created!", "info")
    return redirect(url_for('index'))
  else:
    try:
      return render_template("create_account.html")
    except Exception as e:
      return str(e); #Renders a page with the error.

@app.route('/locked', methods=['GET'])
def locked_access():
  return render_template('locked.html')