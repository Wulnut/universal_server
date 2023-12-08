#!/usr/bin/env python

import socket
import json
import base64
import uuid
import time
import threading
import multiprocessing

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

    data = "universal server"
    byte_string = data.encode("utf-8")
    base64_encoded = base64.b64encode(byte_string).decode("utf-8")

    send = {
        "code":1002,
        "sequence": sequence,
        "time": str(times),
        "data": base64_encoded
    }

    json_object = json.dumps(send, indent = 4)

    for i in range(1):
        client.send(json_object.encode("utf-8"))
        msg = client.recv(1024)
        print("receive: ", msg.decode("utf-8"))

    client.close()

def worker(thread_id):
    print(f"Thread {thread_id} is running")


if __name__ == '__main__':

    num_threads = multiprocessing.cpu_count() * 2
    threads = []

    for i in range(num_threads):
        thread = threading.Thread(target = tcp_conn, args = (i, ))
        threads.append(thread)
        thread.start()
    
    for thread in threads:
        thread.join()


    
