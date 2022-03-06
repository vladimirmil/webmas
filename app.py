from flask import Flask, render_template, url_for, escape, request, jsonify, request
from random import sample
from flask_pymongo import PyMongo
from pymongo import MongoClient

app = Flask(__name__)
client = MongoClient()
num = 20
information = 'string'

db = client['baza']
collection = db.kolekcija


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/data1")
def getdata():
    l = []
    for x in range(num):   
        l.append(collection.find_one({"count":collection.count_documents({})-int(x)}))
    return jsonify(l)


@app.route('/this-route', methods=['GET', 'POST'])
def thisRoute():
    global information
    information = request.data
    print(information)
    return information


@app.route("/data2")
def getdata2():
    info = str(information)
    info = info.replace("'", "")
    info = info.replace("b", "")
    l = collection.find_one({"_id":info})
    return jsonify(l)


if __name__ == "__main__":
    app.run(debug=True)
