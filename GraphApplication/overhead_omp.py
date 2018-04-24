import os
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt

DATASET = [4039,5000,5881,10000]
PROCESSOR = [1,2,4,10,20,40]

bfs_sequential = dict()
bfs_parral = dict()
bfs_overhead = dict()

pr_sequential = dict()
pr_parral = dict()
pr_overhead = dict()

sssp_sequential = dict()
sssp_parral = dict()
sssp_overhead = dict()

for ds in DATASET:
	bfs_parral[ds] = dict()
	bfs_overhead[ds] = dict()
	pr_parral[ds] = dict()
	pr_overhead[ds] = dict()
	sssp_parral[ds] = dict()
	sssp_overhead[ds] = dict()
	for p in PROCESSOR:
		bfs_parral[ds][p] = list()
		pr_parral[ds][p] = list()
		sssp_parral[ds][p] = list()

bfs_openmp = open('./bfs/out_bfs_omp.txt','r')
bfs_openmp_line = bfs_openmp.readlines()

pr_openmp = open('./pr/out_pr_omp.txt','r')
pr_openmp_line = pr_openmp.readlines()

sssp_openmp = open('./sssp/out_sssp_omp.txt','r')
sssp_openmp_line = sssp_openmp.readlines()

index = 0
for line in bfs_openmp_line:
	line = line.strip()
	if index == 0:
		bfs_sequential[4039] = float(line)
	elif index == 1:
		bfs_sequential[5000] = float(line)
	elif index == 2:
		bfs_sequential[5881] = float(line)
	elif index == 3:
		bfs_sequential[10000] = float(line)
	else:
		index_new = index - 4
		offset = index_new % 24 
		dataset_offset = (int) (offset / 6)
		processor_offset = offset - dataset_offset * 6
		# print(dataset_offset,processor_offset,line)
		bfs_parral[DATASET[dataset_offset]][PROCESSOR[processor_offset]].append(float(line))

	index += 1
index = 0
index_n = 0
for line in pr_openmp_line:
	line = line.strip()
	if index % 2 == 1:
		if index_n == 0:
			pr_sequential[4039] = float(line)
		elif index_n == 1:
			pr_sequential[5000] = float(line)
		elif index_n == 2:
			pr_sequential[5881] = float(line)
		elif index_n == 3:
			pr_sequential[10000] = float(line)
		else:
			index_new = index_n - 4
			offset = index_new % 24
			dataset_offset = (int)(offset / 6)
			processor_offset = offset - dataset_offset * 6
			# print(index_new,dataset_offset,processor_offset,line)
			pr_parral[DATASET[dataset_offset]][PROCESSOR[processor_offset]].append(float(line))
		index_n += 1
	index += 1

index = 0
for line in sssp_openmp_line:
	line = line.strip()
	if index == 0:
		sssp_sequential[4039] = float(line)
	elif index == 1:
		sssp_sequential[5000] = float(line)
	elif index == 2:
		sssp_sequential[5881] = float(line)
	elif index == 3:
		sssp_sequential[10000] = float(line)
	else:
		index_new = index - 4
		offset = index_new % 24 
		dataset_offset = (int) (offset / 6)
		processor_offset = offset - dataset_offset * 6
		# print(dataset_offset,processor_offset,line)
		sssp_parral[DATASET[dataset_offset]][PROCESSOR[processor_offset]].append(float(line))

	index += 1


bfs_plot = dict()
sssp_plot = dict()
pr_plot = dict()

bfs_time_plot = dict()
pr_time_plot = dict()
sssp_time_plot = dict()

bfs_time_acc = dict()
pr_time_acc = dict()
sssp_time_acc = dict()

thread_cost = list()
x = ['p=1','p=2','p=4','p=10','p=20','p=40']
for ds in DATASET:
	bfs_plot[ds] = list()
	sssp_plot[ds] = list()
	pr_plot[ds] = list()

	bfs_time_plot[ds] = dict()
	pr_time_plot[ds] = dict()
	sssp_time_plot[ds] = dict()

	bfs_time_acc[ds] = dict()
	pr_time_acc[ds] = dict()
	sssp_time_acc[ds] = dict()	

	for p in PROCESSOR:
		# print(pr_parral[ds][p])
		bfs_parral[ds][p] = round(sum(bfs_parral[ds][p])/len(bfs_parral[ds][p]),4)
		bfs_time_plot[ds][p] = bfs_parral[ds][p]
		bfs_time_acc[ds][p] = round(bfs_sequential[ds]/bfs_parral[ds][p],4)

		pr_parral[ds][p] = round(sum(pr_parral[ds][p])/len(pr_parral[ds][p]),4)
		pr_time_plot[ds][p] = pr_parral[ds][p]
		pr_time_acc[ds][p] = round(pr_sequential[ds]/pr_parral[ds][p],4)

		sssp_parral[ds][p] = round(sum(sssp_parral[ds][p])/len(sssp_parral[ds][p]),4)
		sssp_time_plot[ds][p] = sssp_parral[ds][p]
		sssp_time_acc[ds][p] = round(sssp_sequential[ds]/sssp_parral[ds][p],4)

		# print(sssp_parral[ds][p], sssp_sequential[ds])
		bfs_overhead[ds][p] =  bfs_parral[ds][p] - round(bfs_sequential[ds] / p, 4)
		pr_overhead[ds][p] = pr_parral[ds][p] - round(pr_sequential[ds] / p, 4)
		sssp_overhead[ds][p] = sssp_parral[ds][p] - round(sssp_sequential[ds] / p, 4)
		if p == 1:
			if bfs_overhead[ds][p] > 0:
				thread_cost.append(bfs_overhead[ds][p])
			if pr_overhead[ds][p] > 0:
				thread_cost.append(pr_overhead[ds][p])
			if sssp_overhead[ds][p] > 0:
				thread_cost.append(sssp_overhead[ds][p])
		bfs_plot[ds].append(bfs_overhead[ds][p])
		pr_plot[ds].append(pr_overhead[ds][p])
		sssp_plot[ds].append(sssp_overhead[ds][p])

		# print("BFS\tDATASET:",ds,"PROCESSOR:",p,"OVERHEAD:",round(bfs_overhead[ds][p],4))
		# print("PR\tDATASET:",ds,"PROCESSOR:",p,"OVERHEAD:",round(pr_overhead[ds][p],4))
		# print("SSSP\tDATASET:",ds,"PROCESSOR:",p,"OVERHEAD:",round(sssp_overhead[ds][p],4))
		# print("sequntial: ",bfs_sequential[ds],"parral: ",bfs_parral[ds][p] )

thread_cost = sum(thread_cost)/len(thread_cost)


print ("System + Thread cost, T(PARALLEL(WITH PROCESSOR = 1)) - T(SEQUENTIAL): ",round(thread_cost,4))
print()
print()
print("+"*40+"BFS TIME"+"+"*41)
print ('----------| ',end="")
for pn in PROCESSOR:
	title = str(pn)
	pre_s = int((10 - len(title))/2)
	after_s = 10 - pre_s - len(title)
	# print(pre_s,after_s)
	title = pre_s * " " + title + after_s * " "
	print(title,'| ',end = "")
print()
for vn in DATASET:
	firstcolumn = "var:"+str(vn)
	pre = int((9 - len(firstcolumn))/2)
	fcolumn = pre * " "+firstcolumn
	fcolumn = fcolumn + (9-len(fcolumn)) * " " 
	print(fcolumn,"| ", end = "")
	for pn in PROCESSOR:
		if pn < vn:
			title = str(bfs_time_plot[vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()

print()
print()
print("+"*38+"BFS TIME ACC"+"+"*39)
print ('----------| ',end="")
for pn in PROCESSOR:
	title = str(pn)
	pre_s = int((10 - len(title))/2)
	after_s = 10 - pre_s - len(title)
	# print(pre_s,after_s)
	title = pre_s * " " + title + after_s * " "
	print(title,'| ',end = "")
print()
for vn in DATASET:
	firstcolumn = "var:"+str(vn)
	pre = int((9 - len(firstcolumn))/2)
	fcolumn = pre * " "+firstcolumn
	fcolumn = fcolumn + (9-len(fcolumn)) * " " 
	print(fcolumn,"| ", end = "")
	for pn in PROCESSOR:
		if pn < vn:
			title = str(bfs_time_acc[vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()

print()
print()
print("+"*41+"PR TIME"+"+"*41)
print ('----------| ',end="")
for pn in PROCESSOR:
	title = str(pn)
	pre_s = int((10 - len(title))/2)
	after_s = 10 - pre_s - len(title)
	# print(pre_s,after_s)
	title = pre_s * " " + title + after_s * " "
	print(title,'| ',end = "")
print()
for vn in DATASET:
	firstcolumn = "var:"+str(vn)
	pre = int((9 - len(firstcolumn))/2)
	fcolumn = pre * " "+firstcolumn
	fcolumn = fcolumn + (9-len(fcolumn)) * " " 
	print(fcolumn,"| ", end = "")
	for pn in PROCESSOR:
		if pn < vn:
			title = str(pr_time_plot[vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()

print()
print()
print("+"*39+"PR TIME ACC"+"+"*39)
print ('----------| ',end="")
for pn in PROCESSOR:
	title = str(pn)
	pre_s = int((10 - len(title))/2)
	after_s = 10 - pre_s - len(title)
	# print(pre_s,after_s)
	title = pre_s * " " + title + after_s * " "
	print(title,'| ',end = "")
print()
for vn in DATASET:
	firstcolumn = "var:"+str(vn)
	pre = int((9 - len(firstcolumn))/2)
	fcolumn = pre * " "+firstcolumn
	fcolumn = fcolumn + (9-len(fcolumn)) * " " 
	print(fcolumn,"| ", end = "")
	for pn in PROCESSOR:
		if pn < vn:
			title = str(pr_time_acc[vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()


print()
print()
print("+"*40+"SSSP TIME"+"+"*40)
print ('----------| ',end="")
for pn in PROCESSOR:
	title = str(pn)
	pre_s = int((10 - len(title))/2)
	after_s = 10 - pre_s - len(title)
	# print(pre_s,after_s)
	title = pre_s * " " + title + after_s * " "
	print(title,'| ',end = "")
print()
for vn in DATASET:
	firstcolumn = "var:"+str(vn)
	pre = int((9 - len(firstcolumn))/2)
	fcolumn = pre * " "+firstcolumn
	fcolumn = fcolumn + (9-len(fcolumn)) * " " 
	print(fcolumn,"| ", end = "")
	for pn in PROCESSOR:
		if pn < vn:
			title = str(sssp_time_plot[vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()

print()
print()
print("+"*38+"SSSP TIME ACC"+"+"*38)
print ('----------| ',end="")
for pn in PROCESSOR:
	title = str(pn)
	pre_s = int((10 - len(title))/2)
	after_s = 10 - pre_s - len(title)
	# print(pre_s,after_s)
	title = pre_s * " " + title + after_s * " "
	print(title,'| ',end = "")
print()
for vn in DATASET:
	firstcolumn = "var:"+str(vn)
	pre = int((9 - len(firstcolumn))/2)
	fcolumn = pre * " "+firstcolumn
	fcolumn = fcolumn + (9-len(fcolumn)) * " " 
	print(fcolumn,"| ", end = "")
	for pn in PROCESSOR:
		if pn < vn:
			title = str(sssp_time_acc[vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()
'''' sssp '''

ind = np.arange(len(x))
width = 0.2
fig, ax = plt.subplots()
y1 = np.array(bfs_plot[4039])
y2 = np.array(bfs_plot[5000])
y3 = np.array(bfs_plot[5881])
y4 = np.array(bfs_plot[10000])
rects1 = ax.bar(ind, y1, width, color='r')
rects2 = ax.bar(ind + width, y2, width, color='y')
rects3 = ax.bar(ind + width * 2, y3, width, color='g')
rects4 = ax.bar(ind + width * 3, y4, width, color='b')

ax.set_ylabel('Overhead')
ax.set_title('Overhead for BFS according to processor')
ax.set_xticks(ind + (width + width +width) / 2)
ax.set_xticklabels(('p:1', 'p:2', 'p:4', 'p:10', 'p:20','p:40'))

ax.legend((rects1,rects2,rects3,rects4), ('DS:4039','DS:5000','DS:5881','DS:10000'))

# plt.show()

fig, ax = plt.subplots()
y1 = np.array(pr_plot[4039])
y2 = np.array(pr_plot[5000])
y3 = np.array(pr_plot[5881])
y4 = np.array(pr_plot[10000])
rects1 = ax.bar(ind, y1, width, color='r')
rects2 = ax.bar(ind + width, y2, width, color='y')
rects3 = ax.bar(ind + width * 2, y3, width, color='g')
rects4 = ax.bar(ind + width * 3, y4, width, color='b')

ax.set_ylabel('Overhead')
ax.set_title('Overhead for PR according to processor')
ax.set_xticks(ind + (width + width +width) / 2)
ax.set_xticklabels(('p:1', 'p:2', 'p:4', 'p:10', 'p:20','p:40'))

ax.legend((rects1,rects2,rects3,rects4), ('pr:4039','pr:5000','pr:5881','pr:10000'))

# plt.show()

fig, ax = plt.subplots()
y1 = np.array(sssp_plot[4039])
y2 = np.array(sssp_plot[5000])
y3 = np.array(sssp_plot[5881])
y4 = np.array(sssp_plot[10000])
rects1 = ax.bar(ind, y1, width, color='r')
rects2 = ax.bar(ind + width, y2, width, color='y')
rects3 = ax.bar(ind + width * 2, y3, width, color='g')
rects4 = ax.bar(ind + width * 3, y4, width, color='b')

ax.set_ylabel('Overhead')
ax.set_title('Overhead for SSSP according to processor')
ax.set_xticks(ind + (width + width +width) / 2)
ax.set_xticklabels(('p:1', 'p:2', 'p:4', 'p:10', 'p:20','p:40'))

ax.legend((rects1,rects2,rects3,rects4), ('sssp:4039','sssp:5000','sssp:5881','sssp:10000'))

# plt.show()






