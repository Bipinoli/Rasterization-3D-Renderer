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
# normalize coordinate in the range 0 to 1

srcFile = sys.argv[1] + '.clean'
finalFile = sys.argv[1] + '.triangular'

srcFile = open(srcFile, "r")
finalFile = open(finalFile, "w")


toDivide = None


def digits(num):
	n = str(int(float(num)))
	return len(n)




for line in srcFile:
	strs = line.strip().split()
	if (len(strs) > 4 and strs[0] == "f"):
		for i in range(3,len(strs)):
			finalFile.write('f ' + strs[1] + " " + strs[i-1] + " " + strs[i] + "\n")
	elif len(strs) > 1 and strs[0] == "f":
		finalFile.write(line)
	else:
		# normalize coordinate to [0 to 1]
		if (len(strs) >= 3):
			if (toDivide == None):
				toDivide = 10**digits(strs[1])
			for i in range(1, len(strs)):
				strs[i] = str(float(strs[i])/toDivide)

		for i in strs:
			finalFile.write(i + " ")
		finalFile.write("\n")


srcFile.close()
finalFile.close()

os.remove(sys.argv[1] + '.clean')