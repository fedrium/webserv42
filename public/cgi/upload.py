#!/usr/bin/env python3

import sys

body = b''
while True:
    data = sys.stdin.buffer.read(1024)
    if not data:
        break
    body += data
# print(data)

body_arr = body.split(b'\r\n')
bound = body_arr[0]
filename = body_arr[1][body_arr[1].rfind(b"=") + 2:-1]


def findend(bound, body):
    for i in range(len(body)):
        if body[i:i+len(bound)+2] == bound + b'--':
            return i
    return -1

end = findend(bound, body)
start = body.find(b'\r\n\r\n') + 4

output = body[start:end]



with open("./database/files/" + (filename).decode('utf-8'), 'wb') as nf:
    nf.write(output)
