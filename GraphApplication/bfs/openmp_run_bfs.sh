#!/bin/bash
rm -f out_bfs_omp.txt
gcc bfs_s.c -o bfs_s
gcc -fopenmp bfs_omp.c -o bfs_omp

TIMEFORMAT=%R
(time ./bfs_s 4039 facebook4039.txt m) &>> out_bfs_omp.txt
(time ./bfs_s 5000 graph5000.txt m) &>> out_bfs_omp.txt
(time ./bfs_s 5881 bitcoin5881.txt m) &>> out_bfs_omp.txt
(time ./bfs_s 10000 udgraph10000.txt m) &>> out_bfs_omp.txt
# create 5 citie
for (( i = 0; i < 10; i++ )); do
	#statements

	(time ./bfs_omp 4039 facebook4039.txt m 1) &>> out_bfs_omp.txt
	(time ./bfs_omp 4039 facebook4039.txt m 2) &>> out_bfs_omp.txt
	(time ./bfs_omp 4039 facebook4039.txt m 4) &>> out_bfs_omp.txt
	(time ./bfs_omp 4039 facebook4039.txt m 10) &>> out_bfs_omp.txt
	(time ./bfs_omp 4039 facebook4039.txt m 20) &>> out_bfs_omp.txt
	(time ./bfs_omp 4039 facebook4039.txt m 40) &>> out_bfs_omp.txt

	(time ./bfs_omp 5000 graph5000.txt m 1) &>> out_bfs_omp.txt
	(time ./bfs_omp 5000 graph5000.txt m 2) &>> out_bfs_omp.txt
	(time ./bfs_omp 5000 graph5000.txt m 4) &>> out_bfs_omp.txt
	(time ./bfs_omp 5000 graph5000.txt m 10) &>> out_bfs_omp.txt
	(time ./bfs_omp 5000 graph5000.txt m 20) &>> out_bfs_omp.txt
	(time ./bfs_omp 5000 graph5000.txt m 40) &>> out_bfs_omp.txt

	(time ./bfs_omp 5881 bitcoin5881.txt m 1) &>> out_bfs_omp.txt
	(time ./bfs_omp 5881 bitcoin5881.txt m 2) &>> out_bfs_omp.txt
	(time ./bfs_omp 5881 bitcoin5881.txt m 4) &>> out_bfs_omp.txt
	(time ./bfs_omp 5881 bitcoin5881.txt m 10) &>> out_bfs_omp.txt
	(time ./bfs_omp 5881 bitcoin5881.txt m 20) &>> out_bfs_omp.txt	
	(time ./bfs_omp 5881 bitcoin5881.txt m 40) &>> out_bfs_omp.txt	

	(time ./bfs_omp 10000 udgraph10000.txt m 1) &>> out_bfs_omp.txt
	(time ./bfs_omp 10000 udgraph10000.txt m 2) &>> out_bfs_omp.txt
	(time ./bfs_omp 10000 udgraph10000.txt m 4) &>> out_bfs_omp.txt
	(time ./bfs_omp 10000 udgraph10000.txt m 10) &>> out_bfs_omp.txt
	(time ./bfs_omp 10000 udgraph10000.txt m 20) &>> out_bfs_omp.txt
	(time ./bfs_omp 10000 udgraph10000.txt m 40) &>> out_bfs_omp.txt

done