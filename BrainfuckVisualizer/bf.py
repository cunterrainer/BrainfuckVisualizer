import sys

f = open("t.txt", "w")

for i in range(len(sys.argv[1])):
    f.write("+" * ord(sys.argv[1][i]))
    f.write(".>")

f.close()