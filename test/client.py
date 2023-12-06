#!/usr/bin/env python

import socket
import json
import base64
import uuid
import time
import threading

host = "127.0.0.1"
port = 6000

def generator_uuid():

    uid = str(uuid.uuid4())
    res = ''.join(uid.split('-'))

    return res

def tcp_conn(i):

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

    for i in range(10000):
        client.send(json_object.encode("utf-8"))
        msg = client.recv(1024)
        print("receive: ", msg.decode("utf-8"))

    client.close()

def worker(thread_id):
    print(f"Thread {thread_id} is running")


if __name__ == '__main__':

    num_threads = 10
    threads = []

    for i in range(num_threads):
        thread = threading.Thread(target = tcp_conn, args = (i, ))
        threads.append(thread)
        thread.start()
    
    for thread in threads:
        thread.join()


    