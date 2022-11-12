from flask import Flask, jsonify, request
from flask import Response, make_response
import subprocess as sub

api = Flask(__name__)


@api.route('/', methods=['GET'])
def rootFunc():
    name = sub.run("hostname",
                   capture_output=True,
                   text=True).stdout
    hello = {
        "message": name.split()[0]
    }

    resp = make_response(jsonify(hello), 200)
    resp.headers['Content-Type'] = "application/json"
    return resp


if __name__ == '__main__':
    api.run(host='localhost', port=8080)
    # api.run(host='0.0.0.0', port=8080)
