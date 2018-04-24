#!/bin/bash
rm -f out_bfs_omp_1.txt
gcc bfs_s.c -o bfs_s
gcc -fopenmp bfs_omp.c -o bfs_omp

TIMEFORMAT=%R
(time ./bfs_s 4039 facebook4039.txt l) &>> out_bfs_omp_1.txt
(time ./bfs_s 5000 graph5000.txt l) &>> out_bfs_omp_1.txt
(time ./bfs_s 5881 bitcoin5881.txt l) &>> out_bfs_omp_1.txt
(time ./bfs_s 10000 udgraph10000.txt l) &>> out_bfs_omp_1.txt
# create 5 citie
for (( i = 0; i < 5; i++ )); do
	#statements

	(time ./bfs_omp 4039 facebook4039.txt l 1) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 4039 facebook4039.txt l 2) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 4039 facebook4039.txt l 4) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 4039 facebook4039.txt l 10) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 4039 facebook4039.txt l 20) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 4039 facebook4039.txt l 40) &>> out_bfs_omp_1.txt

	(time ./bfs_omp 5000 graph5000.txt l 1) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 5000 graph5000.txt l 2) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 5000 graph5000.txt l 4) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 5000 graph5000.txt l 10) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 5000 graph5000.txt l 20) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 5000 graph5000.txt l 40) &>> out_bfs_omp_1.txt

	(time ./bfs_omp 5881 bitcoin5881.txt l 1) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 5881 bitcoin5881.txt l 2) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 5881 bitcoin5881.txt l 4) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 5881 bitcoin5881.txt l 10) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 5881 bitcoin5881.txt l 20) &>> out_bfs_omp_1.txt	
	(time ./bfs_omp 5881 bitcoin5881.txt l 40) &>> out_bfs_omp_1.txt	

	(time ./bfs_omp 10000 udgraph10000.txt l 1) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 10000 udgraph10000.txt l 2) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 10000 udgraph10000.txt l 4) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 10000 udgraph10000.txt l 10) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 10000 udgraph10000.txt l 20) &>> out_bfs_omp_1.txt
	(time ./bfs_omp 10000 udgraph10000.txt l 40) &>> out_bfs_omp_1.txt

done