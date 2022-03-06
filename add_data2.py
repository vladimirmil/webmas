from re import template
from pymongo import MongoClient
import time
import json

client = MongoClient()

db = client['baza']
collection = db.kolekcija

while(1):
    data_dict = {}
    i = collection.count_documents({}) + 1
    
    with open('weatherdata.txt', 'r') as f:
        sensor_data = f.read().splitlines()

    data_dict['count'] = i
    data_dict['_id'] = sensor_data[0]
    data_dict['soilhumidity'] = sensor_data[1]
    data_dict['airhumidity'] = sensor_data[2]
    data_dict['temperature'] = sensor_data[3]
    data_dict['illuminance'] = sensor_data[4]

    json_data = json.dumps(data_dict, indent = 4)

    try:
        doc = json.loads(json_data)
        try:
            collection.insert_one(doc)
            print("Dodato: " + str(i))
        except:
            print("Vec postoji takav id")
        pass
    except:
        print("Podaci nisu uspesno ucitani")
        pass
    
    time.sleep(1)



#    with open('testfile.txt', 'w') as file:
#        file.write(json.dumps(data_dict))
