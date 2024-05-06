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
    print("<!DOCTYPE html>")
    print("<html>")
    print("<head>")
    print("<title>Account exists</title>")
    print("<script>")
    print("function redirectToPage() {")
    print("window.location.href = \"/index.html\";")
    print("}")
    print("</script>")
    print("</head>")
    print("<body>")
    print("<h2>Account Exists! Registration not processed!</h2>")
    print("<body>")
    print("<button onclick=\"redirectToPage()\">Go back</button>")
    print("</body>")
    print("</html>")
else:
    file = open("./database/login.csv", "a+")
    file.write(search)
    file.close

    print("<!DOCTYPE html>")
    print("<html>")
    print("<head>")
    print("<title>Registration Successful</title>")
    print("<script>")
    print("function redirectToPage() {")
    print("window.location.href = \"/index.html\";")
    print("}")
    print("</script>")
    print("</head>")
    print("<body>")
    print("<h2>Registration successful!</h2>")
    print("<body>")
    print("<button onclick=\"redirectToPage()\">Go back</button>")
    print("</body>")
    print("</html>")