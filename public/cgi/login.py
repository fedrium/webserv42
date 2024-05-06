#!/usr/bin/env python3

import os

env = os.environ

vars = {}

if env:
    for key, value in env.items():
        name, value = key, value
        vars[name] = value

username = vars.get("username", "")
password = vars.get("password", "")
search = username + "," + password + "\n"
nextAction = "NF"

file = open("./database/login.csv", "r")
if any(line == search for line in file):
    nextAction = "F"
else:
    nextAction = "NF"
file.close()

if (nextAction == "F"):
    if (os.path.isfile("./public/html/upload.html")):
        uploadFilePath = "/upload.html"
    else:
        uploadFilePath = "/cgi/upload.html"

    print("HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: text/html\r\n")
    print("<!DOCTYPE html>")
    print("<html>")
    print("<head>")
    print("<title>Login Successful</title>")
    print("<script>")
    print("function redirectToPage() {")
    print("window.location.href = \"" + uploadFilePath + "\";")
    print("}")
    print("</script>")
    print("</head>")
    print("<body>")
    print("<h2>Login Successful!</h2>")
    print("<body>")
    print("<button onclick=\"redirectToPage()\">To upload</button>")
    print("</body>")
    print("</html>")
else:
    print("<!DOCTYPE html>")
    print("<html>")
    print("<head>")
    print("<title>Login Failed</title>")
    print("<script>")
    print("function redirectToPage() {")
    print("window.location.href = \"/index.html\";")
    print("}")
    print("</script>")
    print("</head>")
    print("<body>")
    print("<h2>Login Failed!</h2>")
    print("<body>")
    print("<button onclick=\"redirectToPage()\">Go back</button>")
    print("</body>")
    print("</html>")