import sys
import re

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