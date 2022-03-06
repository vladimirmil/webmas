from pymongo import MongoClient
import time
import json

client = MongoClient()

db = client['baza']
collection = db.kolekcija

while(1):
    i = collection.count_documents({}) + 1
    
    f = open('weatherdata.txt','r')
    f_contents = f.read()
    f.close()

    add_count = f_contents + "   \"count\":" + str(i) + "\n}"

    try:
        doc = json.loads(add_count)
        try:
            collection.insert_one(doc)
        except:
            print("Vec postoji takav id")
        #pass
    except:
        print("Podaci nisu uspesno ucitani")
        #pass
    
    time.sleep(1)
#with open('weatherdata.txt', 'r') as f: