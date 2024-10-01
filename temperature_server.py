from flask import Flask, jsonify, render_template
import requests

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('./index.html')  

@app.route('/get-temperature')
def get_temperature():
    response = requests.get('http://192.168.211.54/temperature')  
    return jsonify(response.json())

if __name__ == '__main__':
    app.run(debug=True)
