import cgi, cgitb
import sys
 
# total arguments
path = "database/files/" + sys.argv[1]

f = open(path, "a")
i = 2
while i in range(len(sys.argv)):
    f.write(sys.argv[i] + "\r\n")
    i += 1
f.close()

# #!C:\Python27\python.exe
# print ("Content-type: text/html\n\n")

# import cgi
# import cgitb; cgitb.enable()
# import os

# form = cgi.FieldStorage()

# fileitem = form["filename"]

# if fileitem.filename:
# 	fn = os.path.basename(fileitem.filename)
# 	open(fn,'wb').write(fileitem.file.read())

# 	message = 'The file was uploaded successfully'
# 	print (message)
# else:
# 	message = 'The file was not uploaded successfully'
# 	print (message)
