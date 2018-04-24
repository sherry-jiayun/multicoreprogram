import os
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt

DATASET = [4039,5000,5881,10000]
PROCESSOR = [1,2,4,10,20,40]
MODE = ['m','l']
bfs_cpu = dict()
pr_cpu = dict()
sssp_cpu = dict()
path = './oprofile_cpu_result_2/'
for m in MODE:
	bfs_cpu[m] = dict()
	pr_cpu[m] = dict()
	sssp_cpu[m] = dict()
	for ds in DATASET:
		bfs_cpu[m][ds] = dict()
		pr_cpu[m][ds] = dict()
		sssp_cpu[m][ds] = dict()
		for p in PROCESSOR:
			bfs_cpu[m][ds][p] = 0
			pr_cpu[m][ds][p] = 0
			sssp_cpu[m][ds][p] = 0

			file_name = 'bfs_omp_'+str(ds)+'_'+str(p)+'_'+m+'_cpu.txt'
			file = open(path+file_name)
			fileline = file.readlines()
			for line in fileline:
				line = ' '.join(line.split())
				linelist = line.split(' ')
				if (len(linelist) == 4):
					if 'bfs_a' in linelist[3]:
						bfs_cpu[m][ds][p] += float(linelist[1])
						bfs_cpu[m][ds][p] = round(bfs_cpu[m][ds][p],4)
						# print (linelist[1],linelist[3])
					if 'bfs_m' in linelist[3]:
						bfs_cpu[m][ds][p] += float(linelist[1])
						bfs_cpu[m][ds][p] = round(bfs_cpu[m][ds][p],4)
						# print (linelist[1],linelist[3])

			file_name = 'pr_omp_'+str(ds)+'_'+str(p)+'_'+m+'_cpu.txt'
			file = open(path+file_name)
			fileline = file.readlines()
			for line in fileline:
				line = ' '.join(line.split())
				linelist = line.split(' ')
				if (len(linelist) == 4):
					if 'pageranka' in linelist[3]:
						pr_cpu[m][ds][p] += float(linelist[1])
						pr_cpu[m][ds][p] = round(pr_cpu[m][ds][p],4)
						# rint (linelist[1],linelist[3])
						# break;
					if 'pagerankm' in linelist[3]:
						pr_cpu[m][ds][p] += float(linelist[1])
						pr_cpu[m][ds][p] = round(pr_cpu[m][ds][p],4)
						# print (linelist[1],linelist[3])
						# break;

			file_name = 'sssp_omp_'+str(ds)+'_'+str(p)+'_'+m+'_cpu.txt'
			file = open(path+file_name)
			fileline = file.readlines()
			for line in fileline:
				line = ' '.join(line.split())
				linelist = line.split(' ')
				if (len(linelist) == 4):
					if 'pageranka' in linelist[3]:
						sssp_cpu[m][ds][p] += float(linelist[1])
						sssp_cpu[m][ds][p] = round(sssp_cpu[m][ds][p],4)
						# print (linelist[1],linelist[3])
						# break;
					if 'pagerankm' in linelist[3]:
						sssp_cpu[m][ds][p] += float(linelist[1])
						sssp_cpu[m][ds][p] = round(sssp_cpu[m][ds][p],4)
						# print (linelist[1],linelist[3])
						# break;

print(" m for matrix, l for adjlist")
print()
print()
print("+"*37+"BFS CPU COST M"+"+"*37)
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
			title = str(bfs_cpu['m'][vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()

print()
print()
print("+"*37+"BFS CPU COST I"+"+"*37)
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
			title = str(bfs_cpu['l'][vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()

print()
print()
print("+"*38+"PR CPU COST M"+"+"*37)
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
			title = str(pr_cpu['m'][vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()

print()
print()
print("+"*38+"PR CPU COST I"+"+"*37)
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
			title = str(pr_cpu['l'][vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()

print()
print()
print("+"*37+"SSSP CPU COST M"+"+"*36)
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
			title = str(sssp_cpu['m'][vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()

print()
print()
print("+"*37+"SSSP CPU COST I"+"+"*36)
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
			title = str(sssp_cpu['l'][vn][pn])
			pre_s = int((10-len(title))/2)
			after_s = 10 - pre_s - len(title)
			title = pre_s * " " + title + after_s * " "
			print(title,"| ",end = "")
	print()
