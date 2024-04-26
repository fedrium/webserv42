#!/usr/bin/python3

content = """
<!DOCTYPE html>
<html>
<head>
  <title>Upload</title>
</head>
<body>
    <form action="/upload" method="post" enctype="multipart/form-data">
    <input type="file" name="fileToUpload" id="fileToUpload">
    <input type="submit" value="Upload">
  </form>
</body>
</html>
"""

print(content)