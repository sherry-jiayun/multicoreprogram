#!/bin/bash
rm -f out_sssp_omp_1.txt
gcc sssp.c -o sssp_s
gcc -fopenmp sssp_omp_s.c -o sssp_omp

TIMEFORMAT=%R
(time ./sssp_s 4039 facebook4039.txt l) &>> out_sssp_omp_1.txt
(time ./sssp_s 5000 graph5000.txt l) &>> out_sssp_omp_1.txt
(time ./sssp_s 5881 bitcoin5881.txt l) &>> out_sssp_omp_1.txt
(time ./sssp_s 10000 udgraph10000.txt l) &>> out_sssp_omp_1.txt
# create 5 citie
for (( i = 0; i < 5; i++ )); do
	#statements

	(time ./sssp_omp 4039 facebook4039.txt l 1) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 4039 facebook4039.txt l 2) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 4039 facebook4039.txt l 4) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 4039 facebook4039.txt l 10) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 4039 facebook4039.txt l 20) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 4039 facebook4039.txt l 40) &>> out_sssp_omp_1.txt

	(time ./sssp_omp 5000 graph5000.txt l 1) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 5000 graph5000.txt l 2) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 5000 graph5000.txt l 4) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 5000 graph5000.txt l 10) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 5000 graph5000.txt l 20) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 5000 graph5000.txt l 40) &>> out_sssp_omp_1.txt

	(time ./sssp_omp 5881 bitcoin5881.txt l 1) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 5881 bitcoin5881.txt l 2) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 5881 bitcoin5881.txt l 4) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 5881 bitcoin5881.txt l 10) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 5881 bitcoin5881.txt l 20) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 5881 bitcoin5881.txt l 40) &>> out_sssp_omp_1.txt	

	(time ./sssp_omp 10000 udgraph10000.txt l 1) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 10000 udgraph10000.txt l 2) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 10000 udgraph10000.txt l 4) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 10000 udgraph10000.txt l 10) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 10000 udgraph10000.txt l 20) &>> out_sssp_omp_1.txt
	(time ./sssp_omp 10000 udgraph10000.txt l 40) &>> out_sssp_omp_1_1.txt

done