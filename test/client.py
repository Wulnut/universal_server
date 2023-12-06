#!/usr/bin/env python

import socket
import json
import base64
import string
import uuid
import time

host = "127.0.0.1"
port = 6000

def generator_uuid():

    uid = str(uuid.uuid4())
    res = ''.join(uid.split('-'))

    return res


if __name__ == '__main__':

    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    client.connect((host, port))

    sequence = generator_uuid()
    times = time.time()

    str = "universal_server"

    send = {
        "code":1002,
        "sequence": sequence,
        "time": times,
        "data": str
    }

    json_object = json.dumps(send, indent = 4)

    client.send(json_object.encode("utf-8"))
    
    msg = client.recv(1024)

    print("receive: ", msg)

    client.close()
    