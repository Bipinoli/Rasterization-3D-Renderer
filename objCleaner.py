import sys
import re
import os

srcFile = sys.argv[1]
destFile = sys.argv[1] + '.clean'

srcFile = open(srcFile, "r")
destFile = open(destFile, "w")

for line in srcFile:
	pattern = re.compile("[(vn .*?\n)(vt .*?\n)(v .*?\n)(f .*?\n)]")
	if pattern.match(line):
		destFile.write(line)
	else:
		destFile.write("\n")

srcFile.close()
destFile.close()



# generate triangluar faces from the polygon face

srcFile = sys.argv[1] + '.clean'
finalFile = sys.argv[1] + '.triangular'

srcFile = open(srcFile, "r")
finalFile = open(finalFile, "w")


for line in srcFile:
	strs = line.strip().split()
	if (len(strs) > 4 and strs[0] == "f"):
		for i in range(3,len(strs)):
			finalFile.write('f ' + strs[1] + " " + strs[i-1] + " " + strs[i] + "\n")
	else:
		finalFile.write(line)


srcFile.close()
finalFile.close()

os.remove(sys.argv[1] + '.clean')