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

print("HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: text/html\r\n")
print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<title>Upload Successful</title>")
print("<script>")
print("function redirectToPage() {")
print("window.location.href = \"/upload.html\";")
print("}")
print("</script>")
print("</head>")
print("<body>")
print("<h2>Upload successful!</h2>")
print("<body>")
print("<button onclick=\"redirectToPage()\">Go back</button>")
print("</body>")
print("</html>")