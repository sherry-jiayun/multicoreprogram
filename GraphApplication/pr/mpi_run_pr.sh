#!/bin/bash
rm -f out_pr_mpi_2.txt
mpicc -std=c99 -o pr_mpi_ui pr_mpi_ui_2.c -lm # para_ui
mpicc -std=c99 -o pr_mpi pr_mpi_2.c -lm # para

TIMEFORMAT=%R

(time ./pr_s 4039 facebook4039.txt m) &>> out_pr_mpi_2.txt
(time ./pr_s 5000 graph5000.txt m) &>> out_pr_mpi_2.txt
(time ./pr_s 5881 bitcoin5881.txt m) &>> out_pr_mpi_2.txt
(time ./pr_s 10000 udgraph10000.txt m) &>> out_pr_mpi_2.txt

for ((i = 0; i < 5; i++)); do
	(time mpirun -n 1 ./pr_mpi_ui 4039 facebook4039.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 2 ./pr_mpi_ui 4039 facebook4039.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 4 ./pr_mpi_ui 4039 facebook4039.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 10 ./pr_mpi_ui 4039 facebook4039.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 20 ./pr_mpi_ui 4039 facebook4039.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 40 ./pr_mpi_ui 4039 facebook4039.txt m) &>> out_pr_mpi_2.txt

	(time mpirun -n 1 ./pr_mpi 4039 facebook4039.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 2 ./pr_mpi 4039 facebook4039.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 4 ./pr_mpi 4039 facebook4039.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 10 ./pr_mpi 4039 facebook4039.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 20 ./pr_mpi 4039 facebook4039.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 40 ./pr_mpi 4039 facebook4039.txt m) &>> out_pr_mpi_2.txt

	(time mpirun -n 1 ./pr_mpi_ui 5000 graph5000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 2 ./pr_mpi_ui 5000 graph5000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 4 ./pr_mpi_ui 5000 graph5000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 10 ./pr_mpi_ui 5000 graph5000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 20 ./pr_mpi_ui 5000 graph5000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 40 ./pr_mpi_ui 5000 graph5000.txt m) &>> out_pr_mpi_2.txt

	(time mpirun -n 1 ./pr_mpi 5000 graph5000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 2 ./pr_mpi 5000 graph5000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 4 ./pr_mpi 5000 graph5000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 10 ./pr_mpi 5000 graph5000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 20 ./pr_mpi 5000 graph5000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 40 ./pr_mpi 5000 graph5000.txt m) &>> out_pr_mpi_2.txt

	(time mpirun -n 1 ./pr_mpi_ui 5881 bitcoin5881.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 2 ./pr_mpi_ui 5881 bitcoin5881.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 4 ./pr_mpi_ui 5881 bitcoin5881.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 10 ./pr_mpi_ui 5881 bitcoin5881.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 20 ./pr_mpi_ui 5881 bitcoin5881.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 40 ./pr_mpi_ui 5881 bitcoin5881.txt m) &>> out_pr_mpi_2.txt

	(time mpirun -n 1 ./pr_mpi 5881 bitcoin5881.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 2 ./pr_mpi 5881 bitcoin5881.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 4 ./pr_mpi 5881 bitcoin5881.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 10 ./pr_mpi 5881 bitcoin5881.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 20 ./pr_mpi 5881 bitcoin5881.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 40 ./pr_mpi 5881 bitcoin5881.txt m) &>> out_pr_mpi_2.txt

	(time mpirun -n 1 ./pr_mpi_ui 10000 udgraph10000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 2 ./pr_mpi_ui 10000 udgraph10000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 4 ./pr_mpi_ui 10000 udgraph10000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 10 ./pr_mpi_ui 10000 udgraph10000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 20 ./pr_mpi_ui 10000 udgraph10000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 40 ./pr_mpi_ui 10000 udgraph10000.txt m) &>> out_pr_mpi_2.txt

	(time mpirun -n 1 ./pr_mpi 10000 udgraph10000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 2 ./pr_mpi 10000 udgraph10000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 4 ./pr_mpi 10000 udgraph10000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 10 ./pr_mpi 10000 udgraph10000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 20 ./pr_mpi 10000 udgraph10000.txt m) &>> out_pr_mpi_2.txt
	(time mpirun -n 40 ./pr_mpi 10000 udgraph10000.txt m) &>> out_pr_mpi_2.txt

done