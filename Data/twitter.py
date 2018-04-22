import os

# change direction here
file = open('../../../twitter_combined.txt')
fileline = file.readlines()
NODE2INDEX = {}
INDEX2NODE = {}
node_index = 0
G = dict()
for line in fileline:
	line = line.strip()
	line = line.split(' ')
	if len(line) != 2:
		pass
	else:
		v1 = line[0]
		v2 = line[1]
		if v1 not in NODE2INDEX.keys():
			NODE2INDEX[v1] = node_index
			INDEX2NODE[node_index] = v1
			G[node_index] = list()
			node_index += 1
		if v2 not in NODE2INDEX.keys():
			NODE2INDEX[v2] = node_index
			INDEX2NODE[node_index] = v2
			G[node_index] = list()
			node_index += 1
		index_v1 = NODE2INDEX[v1]
		index_v2 = NODE2INDEX[v2]
		if index_v1 not in G[index_v2]:
			G[index_v2].append(index_v1)
		if index_v2 not in G[index_v1]:
			G[index_v1].append(index_v2)
try:
	os.remove("twitter"+str(node_index)+".txt")
except OSError:
	pass

with open("twitter"+str(node_index) +".txt","a") as myfile:
	for i in range(node_index):
		keyList = G[i]
		currentList = list()
		for k in range(node_index):
			if k not in keyList:
				currentList.append(str(0))
			else:
				currentList.append(str(1))
		line = " ".join(currentList)
		line += "\n"
		myfile.write(line)
