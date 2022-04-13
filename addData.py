# -*- coding: utf-8 -*-
import sqlite3
import serial.tools.list_ports
from datetime import datetime
import os

serialNumber = "E669BCF8E7855F2F"
port = ""
out = ""
buff = ""
arr = []
data_count = 0
table = "garden"
fields = """count           INTEGER     NOT NULL UNIQUE,
            _id             TEXT        NOT NULL UNIQUE PRIMARY KEY,
            soilhumidity    INTEGER     NOT NULL,
            airhumidity     REAL        NOT NULL,
            temperature     REAL        NOT NULL,
            lightintensity  INTEGER     NOT NULL"""

ser = serial.Serial()

def create_table(t, f):
    try:
        conn = sqlite3.connect(os.path.realpath('garden.db'))
        c = conn.cursor()
        sql = "CREATE TABLE " + t + "(" + f + ");" 
        c.execute(sql)
        conn.commit()
        conn.close()
        print("Create table: SUCCESS")
    except:
        conn.close()
        print("Create table: FAIL")


def insert_data(t, d):
    try:
        conn = sqlite3.connect(os.path.realpath('garden.db'))
        c = conn.cursor()
        sql = "INSERT INTO " + t + " VALUES(" + d + ")"
        c.execute(sql)
        conn.commit()
        conn.close()
        print("Insert data  : SUCCESS")
    except:
        conn.close()
        print("Insert data  : FAIL")


def get_data(t, what):
    try:
        conn = sqlite3.connect(os.path.realpath('garden.db'))
        c = conn.cursor()
        sql = "SELECT * FROM " + t + " WHERE " + what + ";"
        c.execute(sql)
        result = str(c.fetchone())
        result = result.replace("(", "")
        result = result.replace("\'", "")
        result = result.replace(")", "")
        arr = result.split(",")
        conn.close()
        print("Select data: SUCCESS")
        return arr
    except:
        conn.close()
        print("Select data: FAIL")


def get_count(t):
    try:
        conn = sqlite3.connect(os.path.realpath('garden.db'))
        c = conn.cursor()
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


def serial_init():
    try:
        ports = list(serial.tools.list_ports.comports())
        for p in ports:
            if p.serial_number == serialNumber:
                port = p.device
        ser.baudrate = 115200
        ser.port = port
        ser.open()
        print("Serial init  : SUCCESS")
    except:
        print("Serial init  : FAIL")


def main():
    global arr, buff

    serial_init()
    create_table(table, fields)

    while 1:
        try:
            if ser.isOpen() and ser.in_waiting:
                response = ser.readline()
                out = str(response.decode('utf'))
            
                for element in range(0, len(out)):
                    if out[element] != ',':
                        buff += out[element]
                    else:
                        arr.append(buff)
                        buff = ""

                
                now = datetime.now()
                dt_string = now.strftime("%d/%m/%Y %H:%M:%S")

                try:
                    data_count = get_count(table) + 1
                except:
                    data_count = 1
                    print("Table not found")
                
                data = str(data_count) + ", \"" + dt_string + "\", " + str(arr[0]) + ", " + str(arr[1]) + ", " + str(arr[2]) + ", " + str(arr[3])
                insert_data(table, data)

                buff = ""
                arr.clear()
        except:
            print("Error reading serial data")
            ser.close()
            break

    ser.close()

if __name__ == "__main__":
    main()

