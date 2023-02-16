from flask import Flask, request
from flask_cors import CORS
import paho.mqtt.client as mqtt
import json


app = Flask(__name__)
CORS(app)

#class to save the 'body' of the incoming message
class Message:
    def __init__(self, topic, payload ):
        self.topic = topic
        self.payload = payload

class Time:
    def __init__(self, hour, minute, hour_and_minute):
        self.hour = hour
        self.minute = minute
        self.hour_and_minute = hour_and_minute

def boolean_to_string(data: bool):
    if data == True:
        return "True"
    else:
        return "False"



#HiveMQ broker and MQTT client initialization (just for testing the connection with the broker)
broker = "127.0.0.1"
port = 1883


#callback functions
def on_connect(client, userdata, flags, rc):
    if rc != 0:
        print("CONNECTION FAILED")

def connect_mqtt():
    client = mqtt.Client("pythonclient")# making an mqtt client with the clientID pythonclient
    client.on_connect = on_connect
    client.connect(broker, port)  # connecting to the MQTT server
    if client.is_connected() == True:
        print("CLIENT CONNECTED")
    else:
        print("CONNECTION FAILED")
    return client


@app.route("/", methods=['POST'])
def index():
    print("Connecting to broker")
    client = connect_mqtt() #using the connect_mqtt function to make an mqtt client
    client.loop_start()
    message = Message("","") #making an object to hold the incoming data
    if request == None:
        return "request is None"
    else:
        requestdata = request.get_json() #convert the JSON object into python dictionary
        message.topic = requestdata['topic'] #save the topic of the incoming messsage in the variable of the 'message' object
        message.payload = requestdata['payload'] #save the payload of the incoming message in the variable of the 'message' object
        if message.topic == "windowBlind/open" or message.topic == "windowBlind/close":
            client.publish(message.topic,message.payload)
            message.topic = ""
            message.payload = ""
        #if message.topic == "windowBlind/close":
            #client.publish(message.topic,message.payload)
            #message.topic = ""
            #message.payload = ""
        if message.topic == "windowBlind/refreshTime":
            newpayload = json.dumps(message.payload) #convert the message object to a json string
            client.publish(message.topic,newpayload)
            message.topic = ""
            message.payload = ""
        return "request is not None"

if __name__ == '__main__':
    app.run(debug=True)
