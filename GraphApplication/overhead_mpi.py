import os
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt

DATASET = [4039,5000,5881,10000]
MODE = ['UI','IU'] # U for not deal and I for deal
MODE_2 = ['UU','II']
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
	for md in MODE:
		bfs_parral[ds][md] = dict()
		bfs_overhead[ds][md] = dict()
		pr_parral[ds][md] = dict()
		pr_overhead[ds][md] = dict()
		sssp_parral[ds][md] = dict()
		sssp_overhead[ds][md] = dict()
		for p in PROCESSOR:
			bfs_parral[ds][md][p] = list()
			pr_parral[ds][md][p] = list()
			sssp_parral[ds][md][p] = list()
	for md in MODE_2:
		bfs_parral[ds][md] = dict()
		bfs_overhead[ds][md] = dict()
		pr_parral[ds][md] = dict()
		pr_overhead[ds][md] = dict()
		sssp_parral[ds][md] = dict()
		sssp_overhead[ds][md] = dict()
		for p in PROCESSOR:
			bfs_parral[ds][md][p] = list()
			pr_parral[ds][md][p] = list()
			sssp_parral[ds][md][p] = list()

bfs_openmp = open('./bfs/out_bfs_mpi.txt','r')
bfs_openmp_2 = open('./bfs/out_bfs_mpi_2.txt','r')
bfs_openmp_line = bfs_openmp.readlines()
bfs_openmp_line_2 = bfs_openmp_2.readlines()

pr_openmp = open('./pr/out_pr_mpi.txt','r')
pr_openmp_line = pr_openmp.readlines()
pr_openmp_2 = open('./pr/out_pr_mpi_2.txt','r')
pr_openmp_line_2 = pr_openmp_2.readlines()

sssp_openmp = open('./sssp/out_sssp_mpi.txt','r')
sssp_openmp_2 = open('./sssp/out_sssp_mpi_2.txt','r')
sssp_openmp_line = sssp_openmp.readlines()
sssp_openmp_line_2 = sssp_openmp_2.readlines()

index = 0

# UI IU
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
		offset = index_new % 48
		dataset_offset = (int) (offset / 12)
		# print (offset, dataset_offset)
		mode_offset = (int) ((offset - dataset_offset * 12)/6)
		processor_offset = offset - dataset_offset * 12 - mode_offset * 6
		# print (offset, dataset_offset,mode_offset,processor_offset)
		# print(dataset_offset,processor_offset,line)
		bfs_parral[DATASET[dataset_offset]][MODE[mode_offset]][PROCESSOR[processor_offset]].append(float(line))

	index += 1
#UU II
index = 0
for line in bfs_openmp_line_2:
	line = line.strip()
	if index == 0:
		pass
		# bfs_sequential[4039] = float(line)
	elif index == 1:
		pass
		#bfs_sequential[5000] = float(line)
	elif index == 2:
		pass
		#bfs_sequential[5881] = float(line)
	elif index == 3:
		pass
		#bfs_sequential[10000] = float(line)
	else:
		index_new = index - 4
		offset = index_new % 48
		dataset_offset = (int) (offset / 12)
		# print (offset, dataset_offset)
		mode_offset = (int) ((offset - dataset_offset * 12)/6)
		processor_offset = offset - dataset_offset * 12 - mode_offset * 6
		# print (offset, dataset_offset,mode_offset,processor_offset)
		# print(dataset_offset,processor_offset,line)
		bfs_parral[DATASET[dataset_offset]][MODE_2[mode_offset]][PROCESSOR[processor_offset]].append(float(line))

	index += 1
# print(bfs_parral)
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
			offset = index_new % 48
			dataset_offset = (int)(offset / 12)
			mode_offset = (int) ((offset - dataset_offset * 12)/6)
			processor_offset = offset - dataset_offset * 12 - mode_offset * 6
			# print(index_new,dataset_offset,processor_offset,line)
			pr_parral[DATASET[dataset_offset]][MODE[mode_offset]][PROCESSOR[processor_offset]].append(float(line))
		index_n += 1
	index += 1

index = 0
index_n = 0
for line in pr_openmp_line_2:
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
			offset = index_new % 48
			dataset_offset = (int)(offset / 12)
			mode_offset = (int) ((offset - dataset_offset * 12)/6)
			processor_offset = offset - dataset_offset * 12 - mode_offset * 6
			# print(index_new,dataset_offset,processor_offset,line)
			pr_parral[DATASET[dataset_offset]][MODE_2[mode_offset]][PROCESSOR[processor_offset]].append(float(line))
		index_n += 1
	index += 1

# sssp

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
		offset = index_new % 48
		dataset_offset = (int) (offset / 12)
		# print (offset, dataset_offset)
		mode_offset = (int) ((offset - dataset_offset * 12)/6)
		processor_offset = offset - dataset_offset * 12 - mode_offset * 6
		# print (offset, dataset_offset,mode_offset,processor_offset)
		# print(dataset_offset,processor_offset,line)
		sssp_parral[DATASET[dataset_offset]][MODE[mode_offset]][PROCESSOR[processor_offset]].append(float(line))

	index += 1

index = 0
for line in sssp_openmp_line_2:
	line = line.strip()
	if index == 0:
		sssp_sequential[4039] += float(line)
		sssp_sequential[4039] /= 2 
	elif index == 1:
		sssp_sequential[5000] += float(line)
		sssp_sequential[5000] /= 2
	elif index == 2:
		sssp_sequential[5881] += float(line)
		sssp_sequential[5881] /= 2
	elif index == 3:
		sssp_sequential[10000] += float(line)
		sssp_sequential[10000] /= 2
	else:
		index_new = index - 4
		offset = index_new % 48
		dataset_offset = (int) (offset / 12)
		# print (offset, dataset_offset)
		mode_offset = (int) ((offset - dataset_offset * 12)/6)
		processor_offset = offset - dataset_offset * 12 - mode_offset * 6
		# print (offset, dataset_offset,mode_offset,processor_offset)
		# print(dataset_offset,processor_offset,line)
		sssp_parral[DATASET[dataset_offset]][MODE_2[mode_offset]][PROCESSOR[processor_offset]].append(float(line))

	index += 1

bfs_plot = dict()
sssp_plot = dict()
pr_plot = dict()

bfs_time_plot = dict()
pr_time_plot = dict()
sssp_time_plot = dict()

bfs_time_plot_w = dict()
pr_time_plot_w = dict()
sssp_time_plot_w = dict()

bfs_time_acc = dict()
pr_time_acc = dict()
sssp_time_acc = dict()

bfs_time_acc_w = dict()
pr_time_acc_w = dict()
sssp_time_acc_w = dict()

x = ['p=1','p=2','p=4','p=10','p=20','p=40']
thread_cost = list()
for ds in DATASET:
	bfs_plot[ds] = dict()
	sssp_plot[ds] = dict()
	pr_plot[ds] = dict()

	bfs_time_plot[ds] = dict()
	pr_time_plot[ds] = dict()
	sssp_time_plot[ds] = dict()

	bfs_time_plot_w[ds] = dict()
	pr_time_plot_w[ds] = dict()
	sssp_time_plot_w[ds] = dict()

	bfs_time_acc[ds] = dict()
	pr_time_acc[ds] = dict()
	sssp_time_acc[ds] = dict()	

	bfs_time_acc_w[ds] = dict()
	pr_time_acc_w[ds] = dict()
	sssp_time_acc_w[ds] = dict()	

	for md in MODE:
		bfs_plot[ds][md] = list()
		pr_plot[ds][md] = list()
		sssp_plot[ds][md] = list()
		for p in PROCESSOR:
			# print(pr_parral[ds][p])
			bfs_parral[ds][md][p] = round(sum(bfs_parral[ds][md][p])/len(bfs_parral[ds][md][p]),4)
			pr_parral[ds][md][p] = round(sum(pr_parral[ds][md][p])/len(pr_parral[ds][md][p]),4)
			sssp_parral[ds][md][p] = round(sum(sssp_parral[ds][md][p])/len(sssp_parral[ds][md][p]),4)

			if md == 'UI':
				bfs_time_plot_w[ds][p] = bfs_parral[ds][md][p]
				bfs_time_acc_w[ds][p] = round(bfs_sequential[ds]/bfs_parral[ds][md][p],4)

				pr_time_plot_w[ds][p] = pr_parral[ds][md][p]
				pr_time_acc_w[ds][p] = round(pr_sequential[ds]/pr_parral[ds][md][p],4)

				sssp_time_plot_w[ds][p] = sssp_parral[ds][md][p]
				sssp_time_acc_w[ds][p] = round(sssp_sequential[ds]/sssp_parral[ds][md][p],4)

			# print(pr_parral[ds][md][p], pr_sequential[ds] / p)
			bfs_overhead[ds][md][p] =  bfs_parral[ds][md][p] - round(bfs_sequential[ds] / p, 4)
			pr_overhead[ds][md][p] = pr_parral[ds][md][p] - round(pr_sequential[ds] / p, 4)
			sssp_overhead[ds][md][p] = sssp_parral[ds][md][p] - round(sssp_sequential[ds] / p, 4)
			bfs_plot[ds][md].append(bfs_overhead[ds][md][p])
			pr_plot[ds][md].append(pr_overhead[ds][md][p])
			sssp_plot[ds][md].append(sssp_overhead[ds][md][p])
	for md in MODE_2:
		bfs_plot[ds][md] = list()
		pr_plot[ds][md] = list()
		sssp_plot[ds][md] = list()
		for p in PROCESSOR:
			# print(pr_parral[ds][p])
			bfs_parral[ds][md][p] = round(sum(bfs_parral[ds][md][p])/len(bfs_parral[ds][md][p]),4)
			pr_parral[ds][md][p] = round(sum(pr_parral[ds][md][p])/len(pr_parral[ds][md][p]),4)
			sssp_parral[ds][md][p] = round(sum(sssp_parral[ds][md][p])/len(sssp_parral[ds][md][p]),4)

			if md == 'II':
				bfs_time_plot[ds][p] = bfs_parral[ds][md][p]
				bfs_time_acc[ds][p] = round(bfs_sequential[ds]/bfs_parral[ds][md][p],4)

				pr_time_plot[ds][p] = pr_parral[ds][md][p]
				pr_time_acc[ds][p] = round(pr_sequential[ds]/pr_parral[ds][md][p],4)

				sssp_time_plot[ds][p] = sssp_parral[ds][md][p]
				sssp_time_acc[ds][p] = round(sssp_sequential[ds]/sssp_parral[ds][md][p],4)

			# print(pr_parral[ds][md][p], pr_sequential[ds] / p)
			bfs_overhead[ds][md][p] =  bfs_parral[ds][md][p] - round(bfs_sequential[ds] / p, 4)
			pr_overhead[ds][md][p] = pr_parral[ds][md][p] - round(pr_sequential[ds] / p, 4)
			sssp_overhead[ds][md][p] = sssp_parral[ds][md][p] - round(sssp_sequential[ds] / p, 4)

			if p == 1:
				if bfs_overhead[ds][md][p] > 0:
					thread_cost.append(bfs_overhead[ds][md][p])
				if pr_overhead[ds][md][p] > 0:
					thread_cost.append(pr_overhead[ds][md][p])
				if sssp_overhead[ds][md][p] > 0:
					thread_cost.append(sssp_overhead[ds][md][p])
			bfs_plot[ds][md].append(bfs_overhead[ds][md][p])
			pr_plot[ds][md].append(pr_overhead[ds][md][p])
			sssp_plot[ds][md].append(sssp_overhead[ds][md][p])
			# print("BFS\tDATASET:",ds,"MD:",md,"PROCESSOR:",p,"OVERHEAD:",round(bfs_overhead[ds][md][p],4))
thread_cost = sum(thread_cost)/len(thread_cost)
print ("System + Thread cost, T(PARALLEL(WITH PROCESSOR = 1)) - T(SEQUENTIAL): ",round(thread_cost,4))

''''table'''

'''print()
print()
print("+"*39+"BFS TIME W"+"+"*40)
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
			title = str(bfs_time_plot_w[vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()

print()
print()
print("+"*37+"BFS TIME ACC W"+"+"*38)
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
			title = str(bfs_time_acc_w[vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()

print()
print()
print("+"*40+"PR TIME W"+"+"*40)
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
			title = str(pr_time_plot_w[vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()

print()
print()
print("+"*38+"PR TIME ACC W"+"+"*38)
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
			title = str(pr_time_acc_w[vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()


print()
print()
print("+"*39+"SSSP TIME W"+"+"*39)
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
			title = str(sssp_time_plot_w[vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()

print()
print()
print("+"*37+"SSSP TIME ACC W"+"+"*37)
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
			title = str(sssp_time_acc_w[vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()'''

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
			title = str(bfs_time_plot_w[vn][pn])
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
			title = str(bfs_time_acc_w[vn][pn])
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
			title = str(pr_time_plot_w[vn][pn])
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
			title = str(pr_time_acc_w[vn][pn])
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
			title = str(sssp_time_plot_w[vn][pn])
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
			title = str(sssp_time_acc_w[vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()


'''' bfs '''
ind = np.arange(len(x))
width = 0.2
fig, ax = plt.subplots(2, 2, sharex='col', sharey='row')
fig.suptitle('BFS', fontsize=18)
for i in range(2):
	for j in range(2):
		y1 = np.array(bfs_plot[DATASET[i*2+j]][MODE[0]])
		y2 = np.array(bfs_plot[DATASET[i*2+j]][MODE[1]])

		y3 = np.array(bfs_plot[DATASET[i*2+j]][MODE_2[0]])
		y4 = np.array(bfs_plot[DATASET[i*2+j]][MODE_2[1]])
		rects1 = ax[i][j].bar(ind, y1, width, color='#577EB7')
		rects2 = ax[i][j].bar(ind + width, y2, width, color = '#C7D5A1')
		rects3 = ax[i][j].bar(ind + width * 2, y3, width, color = '#F1C297')
		rects4 = ax[i][j].bar(ind + width * 3, y4, width, color = '#CF9996')

		ax[i][j].set_ylabel('Overhead')
		#ax.set_title('Overhead for bfs according to processor')
		ax[i][j].set_xticks(ind + (width * 3) / 2)
		ax[i][j].set_xticklabels(('p:1', 'p:2', 'p:4', 'p:10', 'p:20','p:40'))

		ax[i][j].legend((rects1,rects2,rects3,rects4), (str(DATASET[i*2+j])+'-ui',str(DATASET[i*2+j])+'-i',str(DATASET[i*2+j])+'-uu',str(DATASET[i*2+j])+'-ii'))
# plt.show()

'''' pr '''
ind = np.arange(len(x))
width = 0.2
fig, ax = plt.subplots(2, 2, sharex='col', sharey='row')
fig.suptitle('PR', fontsize=18)
for i in range(2):
	for j in range(2):
		y1 = np.array(pr_plot[DATASET[i*2+j]][MODE[0]])
		y2 = np.array(pr_plot[DATASET[i*2+j]][MODE[1]])

		y3 = np.array(pr_plot[DATASET[i*2+j]][MODE_2[0]])
		y4 = np.array(pr_plot[DATASET[i*2+j]][MODE_2[1]])
		rects1 = ax[i][j].bar(ind, y1, width, color='#577EB7')
		rects2 = ax[i][j].bar(ind + width, y2, width, color = '#C7D5A1')
		rects3 = ax[i][j].bar(ind + width * 2, y3, width, color = '#F1C297')
		rects4 = ax[i][j].bar(ind + width * 3, y4, width, color = '#CF9996')

		ax[i][j].set_ylabel('Overhead')
		#ax.set_title('Overhead for bfs according to processor')
		ax[i][j].set_xticks(ind + (width * 3) / 2)
		ax[i][j].set_xticklabels(('p:1', 'p:2', 'p:4', 'p:10', 'p:20','p:40'))

		ax[i][j].legend((rects1,rects2,rects3,rects4), (str(DATASET[i*2+j])+'-ui',str(DATASET[i*2+j])+'-i',str(DATASET[i*2+j])+'-uu',str(DATASET[i*2+j])+'-ii'))

# plt.show()

# sssp
ind = np.arange(len(x))
width = 0.2
fig, ax = plt.subplots(2, 2, sharex='col', sharey='row')
fig.suptitle('SSSP', fontsize=18)
for i in range(2):
	for j in range(2):
		y1 = np.array(sssp_plot[DATASET[i*2+j]][MODE[0]])
		y2 = np.array(sssp_plot[DATASET[i*2+j]][MODE[1]])

		y3 = np.array(sssp_plot[DATASET[i*2+j]][MODE_2[0]])
		y4 = np.array(sssp_plot[DATASET[i*2+j]][MODE_2[1]])
		rects1 = ax[i][j].bar(ind, y1, width, color='#577EB7')
		rects2 = ax[i][j].bar(ind + width, y2, width, color = '#C7D5A1')
		rects3 = ax[i][j].bar(ind + width * 2, y3, width, color = '#F1C297')
		rects4 = ax[i][j].bar(ind + width * 3, y4, width, color = '#CF9996')

		ax[i][j].set_ylabel('Overhead')
		#ax.set_title('Overhead for bfs according to processor')
		ax[i][j].set_xticks(ind + (width * 3) / 2)
		ax[i][j].set_xticklabels(('p:1', 'p:2', 'p:4', 'p:10', 'p:20','p:40'))

		ax[i][j].legend((rects1,rects2,rects3,rects4), (str(DATASET[i*2+j])+'-ui',str(DATASET[i*2+j])+'-i',str(DATASET[i*2+j])+'-uu',str(DATASET[i*2+j])+'-ii'))

# plt.show()