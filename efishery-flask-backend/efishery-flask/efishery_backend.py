from flask import Flask, request, jsonify
from flask_sqlalchemy import SQLAlchemy
import paho.mqtt.client as mqtt
import json

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///sensor_data.db'  # SQLite database file path
db = SQLAlchemy(app)

class SensorData(db.Model):
    deviceId = db.Column(db.Integer, primary_key=True)
    temperature = db.Column(db.Float)
    humidity = db.Column(db.Float)
    co2 = db.Column(db.Float)

class Device(db.Model):
    deviceId = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(50))
    location = db.Column(db.String(50))

mqtt_broker = "tailor.cloudmqtt.com"  # MQTT broker address
mqtt_port = 11933  # MQTT broker port
mqtt_sub_topic = "/environment"  # MQTT topic to subscribe
mqtt_username = "sobinwaj"
mqtt_password = "32GHLBqgoufD"

def on_message(client, userdata, msg):
    try:
        data = json.loads(msg.payload)
        temperature = data.get('temperature')
        humidity = data.get('humidity')
        co2 = data.get('co2')

        sensor_data = SensorData(temperature=temperature, humidity=humidity, co2=co2)
        db.session.add(sensor_data)
        db.session.commit()
    except Exception as e:
        print("Error:", str(e))

mqtt_client = mqtt.Client()
mqtt_client.username_pw_set(mqtt_username, mqtt_password)
mqtt_client.on_message = on_message
mqtt_client.connect(mqtt_broker, mqtt_port)
mqtt_client.subscribe(mqtt_sub_topic)
mqtt_client.loop_start()

@app.before_request
def create_database():
    db.create_all()

@app.route('/sensorData', methods=['POST'])
def save_sensor_data():
    data = request.json
    temperature = data.get('temperature')
    humidity = data.get('humidity')
    co2 = data.get('co2')

    sensor_data = SensorData(temperature=temperature, humidity=humidity, co2=co2)
    db.session.add(sensor_data)
    db.session.commit()

    return jsonify({'sucess':'Sensor data saved successfully'}), 200

@app.route('/devices', methods=['POST'])
def register_device():
    data = request.json
    name = data.get('name')
    location = data.get('location')

    device = Device(name=name, location=location)
    db.session.add(device)
    db.session.commit()

    return jsonify({'sucess':'Device registered successfully'}), 200

@app.route('/sensorData', methods=['GET'])
def get_all_sensor_data():
    all_sensor_data = SensorData.query.all()
    sensor_data_list = []
    for sensor_data in all_sensor_data:
        sensor_data_dict = {
            'id': sensor_data.id,
            'temperature': sensor_data.temperature,
            'humidity': sensor_data.humidity,
            'co2': sensor_data.co2
        }
        sensor_data_list.append(sensor_data_dict)

    return jsonify(sensor_data_list), 200

@app.route('/devices', methods=['GET'])
def get_all_devices():
    all_devices = Device.query.all()
    device_list = []
    for device in all_devices:
        device_dict = {
            'id': device.id,
            'name': device.name,
            'location': device.location
        }
        device_list.append(device_dict)

    return jsonify(device_list), 200

if __name__ == '__main__':
    app.run()
