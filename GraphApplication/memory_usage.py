


DATASET = [4039,5000,5881,10000]
PROCESSOR = [1,2,4,10,20,40]
MODE = ['m','l']
pr_memory = dict()
sssp_memory = dict()

for m in MODE:
	pr_memory[m] = dict()
	sssp_memory[m] = dict()
	for ds in DATASET:
		pr_memory[m][ds] = dict()
		sssp_memory[m][ds] = dict()
pr['m'][4039][20] = 99.87
pr['m'][4039][40] = 99.87
pr['l'][4039][2] = 44.89 - 29.88
pr['l'][4039][4] = 46.72 - 29.88
pr['l'][4039][20] = 44.18 - 29.88
pr['l'][4039][10] = 44.11 - 29.88
