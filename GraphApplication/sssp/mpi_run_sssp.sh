#!/bin/bash
rm -f out_sssp_mpi_1.txt
mpicc -std=c99 -o sssp_mpi sssp_mpi_2.c -lm # para

TIMEFORMAT=%R

(time ./sssp_s 4039 facebook4039.txt l) &>> out_sssp_mpi_1.txt
(time ./sssp_s 5000 graph5000.txt l) &>> out_sssp_mpi_1.txt
(time ./sssp_s 5881 bitcoin5881.txt l) &>> out_sssp_mpi_1.txt
(time ./sssp_s 10000 udgraph10000.txt l) &>> out_sssp_mpi_1.txt

for ((i = 0; i < 5; i++)); do
	
	(time mpirun -n 1 ./sssp_mpi 4039 facebook4039.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 2 ./sssp_mpi 4039 facebook4039.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 4 ./sssp_mpi 4039 facebook4039.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 10 ./sssp_mpi 4039 facebook4039.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 20 ./sssp_mpi 4039 facebook4039.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 40 ./sssp_mpi 4039 facebook4039.txt l) &>> out_sssp_mpi_1.txt

	(time mpirun -n 1 ./sssp_mpi 5000 graph5000.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 2 ./sssp_mpi 5000 graph5000.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 4 ./sssp_mpi 5000 graph5000.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 10 ./sssp_mpi 5000 graph5000.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 20 ./sssp_mpi 5000 graph5000.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 40 ./sssp_mpi 5000 graph5000.txt l) &>> out_sssp_mpi_1.txt

	(time mpirun -n 1 ./sssp_mpi 5881 bitcoin5881.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 2 ./sssp_mpi 5881 bitcoin5881.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 4 ./sssp_mpi 5881 bitcoin5881.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 10 ./sssp_mpi 5881 bitcoin5881.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 20 ./sssp_mpi 5881 bitcoin5881.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 40 ./sssp_mpi 5881 bitcoin5881.txt l) &>> out_sssp_mpi_1.txt

	(time mpirun -n 1 ./sssp_mpi 10000 udgraph10000.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 2 ./sssp_mpi 10000 udgraph10000.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 4 ./sssp_mpi 10000 udgraph10000.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 10 ./sssp_mpi 10000 udgraph10000.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 20 ./sssp_mpi 10000 udgraph10000.txt l) &>> out_sssp_mpi_1.txt
	(time mpirun -n 40 ./sssp_mpi 10000 udgraph10000.txt l) &>> out_sssp_mpi_1.txt

done