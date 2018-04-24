#!/bin/bash
rm -f out_bfs_mpi_1.txt
mpicc -std=c99 -o bfs_mpi bfs_mpi_2.c -lm # para

TIMEFORMAT=%R

(time ./bfs_s 4039 facebook4039.txt l) &>> out_bfs_mpi_1.txt
(time ./bfs_s 5000 graph5000.txt l) &>> out_bfs_mpi_1.txt
(time ./bfs_s 5881 bitcoin5881.txt l) &>> out_bfs_mpi_1.txt
(time ./bfs_s 10000 udgraph10000.txt l) &>> out_bfs_mpi_1.txt

for ((i = 0; i < 5; i++)); do

	(time mpirun -n 1 ./bfs_mpi 4039 facebook4039.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 2 ./bfs_mpi 4039 facebook4039.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 4 ./bfs_mpi 4039 facebook4039.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 10 ./bfs_mpi 4039 facebook4039.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 20 ./bfs_mpi 4039 facebook4039.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 40 ./bfs_mpi 4039 facebook4039.txt l) &>> out_bfs_mpi_1.txt

	(time mpirun -n 1 ./bfs_mpi 5000 graph5000.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 2 ./bfs_mpi 5000 graph5000.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 4 ./bfs_mpi 5000 graph5000.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 10 ./bfs_mpi 5000 graph5000.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 20 ./bfs_mpi 5000 graph5000.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 40 ./bfs_mpi 5000 graph5000.txt l) &>> out_bfs_mpi_1.txt


	(time mpirun -n 1 ./bfs_mpi 5881 bitcoin5881.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 2 ./bfs_mpi 5881 bitcoin5881.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 4 ./bfs_mpi 5881 bitcoin5881.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 10 ./bfs_mpi 5881 bitcoin5881.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 20 ./bfs_mpi 5881 bitcoin5881.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 40 ./bfs_mpi 5881 bitcoin5881.txt l) &>> out_bfs_mpi_1.txt

	(time mpirun -n 1 ./bfs_mpi 10000 udgraph10000.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 2 ./bfs_mpi 10000 udgraph10000.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 4 ./bfs_mpi 10000 udgraph10000.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 10 ./bfs_mpi 10000 udgraph10000.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 20 ./bfs_mpi 10000 udgraph10000.txt l) &>> out_bfs_mpi_1.txt
	(time mpirun -n 40 ./bfs_mpi 10000 udgraph10000.txt l) &>> out_bfs_mpi_1.txt

done