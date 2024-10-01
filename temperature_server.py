from flask import Flask, jsonify, render_template
import requests

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')  # Asegúrate de tener 'index.html' en el directorio 'templates'

@app.route('/get-temperature')
def get_temperature():
    # Hacer la solicitud al ESP32
    response = requests.get('http://192.168.211.54/temperature')  
    data = response.json()

    # Procesar el JSON que viene del ESP32
    temperature_1 = data.get('temperature_1')
    temperature_2 = data.get('temperature_2')

    # Devolver ambos valores en la respuesta JSON
    return jsonify({
        'sensor_1': temperature_1,
        'sensor_2': temperature_2
    })

if __name__ == '__main__':
    app.run(debug=True)
