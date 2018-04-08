import networkx as nx
import os
import sys
import random

if __name__ == '__main__':

	# get args # file name; nodes's number; directed?
	if len(sys.argv) != 3:
		print("Wrong para! use as python graph_generated.py numberOfNode directed")
		exit()
	try:
		num_node = int(sys.argv[1])
	except ValueError:
		print("Number of node should be an integer.")
		exit()

	if not sys.argv[2] == '0' and not sys.argv[2] == '1':
		print("Illegal para: 1 for directed graph and 0 for undirected. ")
	elif sys.argv[2] == '0':
		directed = False
	else:
		directed = True

	if directed:
		try:
			os.remove("graph"+str(num_node)+".txt")
		except OSError:
			pass
		DG = nx.fast_gnp_random_graph(num_node,0.2,100,True)
		with open("graph"+str(num_node)+".txt", "a") as myfile:	
			for i in range(num_node):
				keyList = list(DG[i].keys())
				currentList = []
				for k in range(num_node):
					if k not in keyList:
						currentList.append(str(0))
					else:
						currentList.append(str(random.randint(1,100)))
				line = " ".join(currentList)
				line += "\n"
				myfile.write(line)
	else:
		try:
			os.remove("udgraph"+str(num_node)+".txt")
		except OSError:
			pass
		weight = dict()
		G = nx.fast_gnp_random_graph(num_node,0.2)
		for i in range(num_node):
			keyList = list(G[i].keys())
			for k in keyList:
				weighttmp = random.randint(1,100)
				if i not in weight.keys():
					weight[i] = dict()
				if k not in weight.keys():
					weight[k] = dict()
				if k not in weight[i].keys():
					weight[i][k] = weighttmp
					weight[k][i] = weighttmp
		with open("udgraph"+str(num_node)+".txt","a") as myfile:
			for i in range(num_node):
				keyList = list(G[i].keys())
				currentList = []
				for k in range(num_node):
					if k not in keyList:
						currentList.append(str(0))
					else:
						currentList.append(str(weight[i][k]))
				line = " ".join(currentList)
				line += "\n"
				myfile.write(line)



