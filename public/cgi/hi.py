#!/usr/bin/env python3

import os

env = os.environ

vars = {}

if env:
    for key, value in env.items():
        # Split each key-value pair using "=" delimiter
        name, value = key, value
        vars[name] = value

username = vars.get("username", "")
password = vars.get("password", "")

print("<html>")
print("<head>")
print("<title>HALLO</title>")
print("</head>")
print("<body>")
print("<h1>Hello " + username + "!</h1>")
print("</br>")
print("<h1>Your password is:" + password + ".</h1>")
print("</body>")
print("</html>")
print('<button onclick="redirectToAnotherPage()">To Upload</button>')
print("<script>")
print("        function redirectToAnotherPage() {")
print("            // Redirect to another HTML file")
print("            window.location.href = 'upload.html';")
print("        }")
print("    </script>")