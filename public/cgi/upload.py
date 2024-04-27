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
