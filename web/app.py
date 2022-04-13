from flask import Flask, render_template,request, jsonify
import sqlite3
import os

app = Flask(__name__)
num = 20
information = 'string'
table = "garden"


def getSqlData(t, what):
    conn = sqlite3.connect(os.path.realpath("garden.db"))
    c = conn.cursor()
    try:
        sql = "SELECT * FROM " + t + " WHERE " + what + ";"
        c.execute(sql)
        result = str(c.fetchone())
        result = result.replace("(", "")
        result = result.replace("\'", "")
        result = result.replace(")", "")
        arr = result.split(",")
        print("Select data: SUCCESS")
        conn.commit()
        conn.close()
        return arr
    except:
        conn.close()
        print("Select data: FAIL")

def get_count(t):
    conn = sqlite3.connect(os.path.realpath("garden.db"))
    c = conn.cursor()
    try:
        sql = "SELECT COUNT(*) FROM "+ t + ";"
        c.execute(sql)
        result = str(c.fetchone())
        result = result.replace("(", "")
        result = result.replace(",", "")
        result = result.replace(")", "")
        conn.commit()
        conn.close()
        print("Get data: SUCCESS")
        return int(result)
    except:
        conn.close()
        print("Get data: FAIL")


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/data1")
def getdata():
    l = []
    for x in range(num):   
        i = "count=" + str(get_count(table) - x)
        l.append(getSqlData(table, i))
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
    i = "count=" + str(get_count(info))
    l = getSqlData(table, i)
    return jsonify(l)


if __name__ == "__main__":
    app.run(debug=True)
