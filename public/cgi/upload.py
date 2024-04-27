#!/usr/bin/python3

print("test")

import os
from flask import request

fi = form['filename']
print("file is: ", os.path.basename(fi.filename))
if fi.filename:
	# This code will strip the leading absolute path from your file-name
	fil = os.path.basename(fi.filename)
	# open for reading & writing the file into the server
	open(fn, 'wb').write(fi.file.read())


# content = """
# <!DOCTYPE html>
# <html>
# <head>
#   <title>Upload</title>
# </head>
# <body>
#     <form action="./public/cgi/upload.py" method="post" enctype="multipart/form-data">
#     <input type="file" name="fileToUpload" id="fileToUpload">
#     <input type="submit" value="Upload">
#   </form>
# </body>
# </html>
# """

# print(content)