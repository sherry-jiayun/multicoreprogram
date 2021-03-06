#include "loadgraph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

/*** global variable ***/

int numOfNode;
float sum_global = -1;
/***
在头文件里引用 "loadgraph.h"
用下面这一段当作main函数
运行的时候使用 ./可执行文件 node数目 文件名 模式(m for matrix l for adj list)
matrix 是 numOfNode * numOfNode的数组
adj 单向list 结构如下
typedef struct edge
{
  int nodefrom;
  int nodeto;
  int weight;
  struct edge * nextedge;
}Edge;

typedef struct node
{
  int nodeNum;
  Edge * edgehead;
}Node;
***/
int pagerankchack(float *prold, float *prnew, int numOfNode, int work_per_processor, int comm_sz){
	float sum = 0;
	for (int i = 0; i < numOfNode; i++){
		float sumtmp = prold[i] - prnew[i];
		if (sumtmp < 0) sum -= sumtmp;
		else sum += sumtmp;
		prold[i] = prnew[i];
	}
	// printf("sum: %f\n", sum );
	if (sum_global - sum < 0.000001 && sum_global - sum > -0.000001) return 0;
	sum_global = sum;
	return 1;
}
void pagerankm(int **graphmatric, int comm_sz, int my_rank){
	// iniitalize pagerank
	float *pagerankinitial_global;
	int *inouttable_global;
	int iter = 0;

	/* work per processor & initialize*/
	float * pagerankinitial_local;
	int *inouttable_local;
	int work_per_processor = numOfNode / comm_sz;
	/* deal with 100 / 40 situation */
	// if (work_per_processor * comm_sz < numOfNode) work_per_processor++;
	pagerankinitial_local = (float *)malloc(work_per_processor * sizeof(float));
	inouttable_local = malloc(work_per_processor * sizeof(int));
	pagerankinitial_global = (float *)malloc(numOfNode* sizeof(float));
	inouttable_global = malloc(numOfNode * sizeof(int));

	float initializeNum = 1/(float)numOfNode;
	for (int i = 0; i < work_per_processor; i++){
		int localindex = work_per_processor * my_rank + i;
		if (localindex >= numOfNode) break;
		pagerankinitial_local[i] = initializeNum;
		inouttable_local[i] = 0;
		for (int j = 0; j < numOfNode; j++){
			if (graphmatric[localindex][j] != 0){
				// printf("%d ", j );
				inouttable_local[i] += 1;
			}
		}
		// printf("\n");
		// printf("Node No.%d out route number %d from rank %d \n",i,inouttable_local[i],my_rank );
	}

	MPI_Gather(pagerankinitial_local, work_per_processor, MPI_FLOAT, pagerankinitial_global, work_per_processor,MPI_FLOAT,0,MPI_COMM_WORLD);
	MPI_Gather(inouttable_local,work_per_processor,MPI_INT, inouttable_global,work_per_processor,MPI_INT,0,MPI_COMM_WORLD);
	if (my_rank == 0){
		for (int i = work_per_processor * comm_sz; i < numOfNode; i++){
			pagerankinitial_global[i] = initializeNum;
			inouttable_global[i] = 0;
			for (int j = 0; j < numOfNode; j++){
				if (graphmatric[i][j] != 0){
					inouttable_global[i] += 1;
				}
			}
		}
	}
	MPI_Bcast(pagerankinitial_global, numOfNode,MPI_FLOAT,0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(inouttable_global,numOfNode,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	float *prnew_global;
	float *prnew_local;
	prnew_global = (float *)malloc(numOfNode * sizeof(float));
	prnew_local = (float *)malloc(work_per_processor * sizeof(float));
	int checkResult = 1;
	while (checkResult == 1){
		iter++;
		for (int i = 0; i < work_per_processor; i++){
			int localindex = work_per_processor * my_rank + i;
			if (localindex >= numOfNode) break;
			prnew_local[i] = 0;
			for (int j = 0; j < numOfNode; j++){
				// maybe able to optimize (for cache hit rate)
				if (graphmatric[j][localindex] != 0){ // node j has edge to i
					// printf("%d %f %d\n",j,pagerankinitial_global[j],inouttable_global[j] );
					prnew_local[i] += pagerankinitial_global[j]/inouttable_global[j];
					// printf("%f for index %d from rank %d\n",prnew_local[i],i,my_rank );
				}
			}
			// printf("Node No.%d old: %f new: %f from rank %d \n",i,pagerankinitial_global[i],prnew_local[i],my_rank );
		}
		MPI_Gather(prnew_local, work_per_processor, MPI_FLOAT, prnew_global,work_per_processor, MPI_FLOAT,0,MPI_COMM_WORLD);
		if (my_rank == 0){
			for (int i = work_per_processor * comm_sz; i < numOfNode; i++){
				prnew_global[i] = 0;
				for (int j = 0; j < numOfNode; j++){
					if (graphmatric[j][i] != 0){
						prnew_global[i] += pagerankinitial_global[j]/inouttable_global[j];
					}
				}
				// printf("Node No.%d old: %f new: %f from rank %d \n",i,pagerankinitial_global[i],prnew_global[i],my_rank );
			}
		}
		MPI_Bcast(prnew_global, numOfNode, MPI_FLOAT, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
		checkResult = pagerankchack(pagerankinitial_global,prnew_global,numOfNode,work_per_processor,comm_sz);
		//break;
	}
	if (my_rank == 0){
		printf("Iteration: %d\n",iter );
	}
}
void pageranka(Node *head, int comm_sz, int my_rank){

	// initialize pagerank
	Node a_node[numOfNode]; // reorganize adj list 
	float *pagerankinitial_global; // initialize pr for every node
	float *pagerankinitial_local;
	int *inouttable_global; // calculate inout edge num for each node 
	int *inouttable_local;
	int iter = 0;
	float initializeNum = 1/(float)numOfNode;
	for (int i = 0; i < numOfNode; i++){
		a_node[i].nodeNum = i;
		a_node[i].edgehead = NULL;
	}

	// initialize setting for mpi
	int work_per_processor = numOfNode / comm_sz;
	if (work_per_processor * comm_sz < numOfNode) work_per_processor++;
	pagerankinitial_global = (float *)malloc( work_per_processor * comm_sz * sizeof(float));
	pagerankinitial_local = (float *)malloc( work_per_processor * sizeof(float));
	inouttable_global = malloc( work_per_processor * comm_sz * sizeof(int));
	inouttable_local = malloc( work_per_processor * sizeof(int));

	// 这里需要思考一下是不是要用 MPI parallel
	// 因为不是基础类型所以很麻烦
	for (int i = 0; i < numOfNode; i++){
		pagerankinitial_global[i] = initializeNum;
		inouttable_global[i] = 0;
		Node ntmp = (Node)head[i];
		Edge *etmp = (Edge *)ntmp.edgehead;

		while (etmp != NULL){
			Edge *a_edge_tmp;
			a_edge_tmp = malloc(sizeof(Edge));
			a_edge_tmp->nodefrom = etmp->nodefrom;
			a_edge_tmp->nodeto = etmp->nodeto;
			a_edge_tmp->weight = etmp->weight;
			a_edge_tmp->nextedge = a_node[etmp->nodeto].edgehead;
			a_node[etmp->nodeto].edgehead = a_edge_tmp;
			inouttable_global[i] += 1;
			etmp = etmp ->nextedge;
		}
		/*if (my_rank == 0){
			printf("Node No.%d out route number %d\n",i,inouttable_global[i] );
		}*/
	}
	float *prnew_global;
	float *prnew_local;
	prnew_global = (float *)malloc(work_per_processor * comm_sz * sizeof(float));
	prnew_local = (float *)malloc(work_per_processor * sizeof(float));
	int checkResult = 1;
	while (checkResult == 1){
		iter++;
		for (int i = 0; i < work_per_processor; i++){
			int localindex = work_per_processor * my_rank + i;
			if (localindex >= numOfNode) break;
			prnew_local[i] = 0;
			Node ntmp = (Node)a_node[localindex];
			Edge *etmp = (Edge *)ntmp.edgehead;
			while(etmp != NULL){
				int fromNode = etmp->nodefrom;
				// printf("%d ",fromNode );
				prnew_local[i] += pagerankinitial_global[fromNode]/inouttable_global[fromNode];
				etmp = etmp ->nextedge;
			}
			// printf("\n");
			// printf("Node No.%d global No.%d old: %f new: %f from rank %d \n",i,localindex, pagerankinitial_global[i],prnew_local[i],my_rank );
		}
		MPI_Allgather(prnew_local, work_per_processor, MPI_FLOAT, prnew_global,work_per_processor, MPI_FLOAT,MPI_COMM_WORLD); 
		/*for (int i = 0; i < numOfNode; i++){
			printf("No,%d: %f ",i,prnew_global[i] );
		}
		printf("From rank %d\n",my_rank);*/
		checkResult = pagerankchack(pagerankinitial_global,prnew_global,numOfNode,work_per_processor,comm_sz);
		// break;
	}
	/* for (int i = 0; i < numOfNode; i++){
		printf("Node No.%d now: %f\n",i,pagerankinitial[i]);
	}*/ 
	if (my_rank == 0){
		printf("Iteration: %d\n",iter );
	}
}

int main(int argc, char *argv[])    
{
	
	char *nameOfFile;
	int index = 0;
	char *mode;
	char mode_matrix[2];
	char mode_adjlist[2];
	strcpy(mode_matrix,"m");
	strcpy(mode_adjlist,"l");

	// initialize mpi_processor
	int comm_sz; 
	int my_rank;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	if(argc == 4){
		numOfNode = getInt(argv[1]);
		if (numOfNode == -1){
			printf("Illegal number of city %s\n",argv[1] );
			MPI_Finalize();
			return 0;
		}
		nameOfFile = argv[2];
		mode = argv[3];
		int result = strcmp(mode_matrix,mode);
		if (result == 0){
			int **graphmatric; // numOfNode * numOfNode matrix 
			graphmatric = malloc(numOfNode * sizeof *graphmatric);
			for (;index < numOfNode;index++){
				graphmatric[index] = malloc(numOfNode * sizeof(int));
			}
			if (my_rank == 0){
				creatMatric(nameOfFile,numOfNode,graphmatric);
			}
			for (int i = 0; i < numOfNode; i ++){
				MPI_Bcast(graphmatric[i], numOfNode, MPI_INT, 0, MPI_COMM_WORLD);
				MPI_Barrier(MPI_COMM_WORLD);
			}
			pagerankm(graphmatric,comm_sz,my_rank);
			free(graphmatric);
		}else{
			result = strcmp(mode_adjlist,mode);
			if (result == 0){
				Node nodelist[numOfNode]; // number of node array
				createAdjList(nameOfFile,numOfNode,(Node *)&nodelist);
				pageranka((Node *)&nodelist, comm_sz, my_rank);
				
			}else{
				printf("Illegal mode!\n");
				MPI_Finalize();
				return 0;
			}
		}
	}else{
		printf("Not have enough para.\n");
		MPI_Finalize();
		return 0;
	}
	MPI_Finalize();
	return 0;
}