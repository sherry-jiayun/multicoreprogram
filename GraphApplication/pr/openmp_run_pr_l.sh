#!/bin/bash
rm -f out_pr_omp_1.txt
gcc pr_s.c -o pr_s
gcc -fopenmp pr_omp.c -o pr_omp

TIMEFORMAT=%R
(time ./pr_s 4039 facebook4039.txt l) &>> out_pr_omp_1.txt
(time ./pr_s 5000 graph5000.txt l) &>> out_pr_omp_1.txt
(time ./pr_s 5881 bitcoin5881.txt l) &>> out_pr_omp_1.txt
(time ./pr_s 10000 udgraph10000.txt l) &>> out_pr_omp_1.txt
# create 5 citie
for (( i = 0; i < 5; i++ )); do
	#statements

	(time ./pr_omp 4039 facebook4039.txt l 1) &>> out_pr_omp_1.txt
	(time ./pr_omp 4039 facebook4039.txt l 2) &>> out_pr_omp_1.txt
	(time ./pr_omp 4039 facebook4039.txt l 4) &>> out_pr_omp_1.txt
	(time ./pr_omp 4039 facebook4039.txt l 10) &>> out_pr_omp_1.txt
	(time ./pr_omp 4039 facebook4039.txt l 20) &>> out_pr_omp_1.txt
	(time ./pr_omp 4039 facebook4039.txt l 40) &>> out_pr_omp_1.txt

	(time ./pr_omp 5000 graph5000.txt l 1) &>> out_pr_omp_1.txt
	(time ./pr_omp 5000 graph5000.txt l 2) &>> out_pr_omp_1.txt
	(time ./pr_omp 5000 graph5000.txt l 4) &>> out_pr_omp_1.txt
	(time ./pr_omp 5000 graph5000.txt l 10) &>> out_pr_omp_1.txt
	(time ./pr_omp 5000 graph5000.txt l 20) &>> out_pr_omp_1.txt
	(time ./pr_omp 5000 graph5000.txt l 40) &>> out_pr_omp_1.txt

	(time ./pr_omp 5881 bitcoin5881.txt l 1) &>> out_pr_omp_1.txt
	(time ./pr_omp 5881 bitcoin5881.txt l 2) &>> out_pr_omp_1.txt
	(time ./pr_omp 5881 bitcoin5881.txt l 4) &>> out_pr_omp_1.txt
	(time ./pr_omp 5881 bitcoin5881.txt l 10) &>> out_pr_omp_1.txt
	(time ./pr_omp 5881 bitcoin5881.txt l 20) &>> out_pr_omp_1.txt
	(time ./pr_omp 5881 bitcoin5881.txt l 40) &>> out_pr_omp_1.txt	

	(time ./pr_omp 10000 udgraph10000.txt l 1) &>> out_pr_omp_1.txt
	(time ./pr_omp 10000 udgraph10000.txt l 2) &>> out_pr_omp_1.txt
	(time ./pr_omp 10000 udgraph10000.txt l 4) &>> out_pr_omp_1.txt
	(time ./pr_omp 10000 udgraph10000.txt l 10) &>> out_pr_omp_1.txt
	(time ./pr_omp 10000 udgraph10000.txt l 20) &>> out_pr_omp_1.txt
	(time ./pr_omp 10000 udgraph10000.txt l 40) &>> out_pr_omp_1.txt

done