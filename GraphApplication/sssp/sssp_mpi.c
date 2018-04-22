#include "loadgraph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>


int numOfNode;
int *tent;
int delta;
int **B;
/***
在头文件里引用 "loadgraph.h"
用下面这一段当作main函数
运行的时候使用 ./可执行文件 node数目 模式(m for matrix l for adj list)
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
void initialize(int **graphmatric){
	// initialize lightEdge
	int weightMax = -1;
	int weightMin = 999999;
	tent = malloc(numOfNode * sizeof(int));
	memset(tent, -1, sizeof(int) * numOfNode); // initialize tentative
	B = malloc(2 * sizeof(int));
	B[0] = malloc(numOfNode * sizeof(int));
	B[1] = malloc(numOfNode * sizeof(int));
	memset(B[0], -1, sizeof(int) * numOfNode);
	memset(B[1], -1, sizeof(int) * numOfNode);
	for(int i = 0; i < numOfNode; i++){
		for (int j = 0; j < numOfNode; j++){
			if (graphmatric[i][j] != 0){
				int weight = graphmatric[i][j];
				if (weight > weightMax) weightMax = weight;
				if (weight < weightMin) weightMin = weight;
			}
		}
	}
	delta = weightMin + (weightMax - weightMin)/2;
}

void initialize_a(Node *head){
	// initialize lightEdge
	int weightMax = -1;
	int weightMin = 999999;
	tent = malloc(numOfNode * sizeof(int));
	memset(tent, -1, sizeof(int) * numOfNode); // initialize tentative
	B = malloc(2 * sizeof(int));
	B[0] = malloc(numOfNode * sizeof(int));
	B[1] = malloc(numOfNode * sizeof(int));
	memset(B[0], -1, sizeof(int) * numOfNode);
	memset(B[1], -1, sizeof(int) * numOfNode);
	for(int i = 0; i < numOfNode; i++){
		Node ntmp = (Node)head[i];
		Edge *etmp = (Edge *)ntmp.edgehead;
		while (etmp != NULL){
			int weight = etmp->weight;
			if (weight > weightMax) weightMax = weight;
			if (weight < weightMin) weightMin = weight;
			etmp = etmp->nextedge;
		}
	}
	delta = weightMin + (weightMax - weightMin)/2;
}

void relax(int w, int tenttmp){
	if (tenttmp < tent[w] || tent[w] == -1){
		tent[w] = tenttmp;
		if (tenttmp / delta == 0){
			for ( int i = 0; i < numOfNode; i++){
				if (B[0][i] == -1 ){
					B[0][i] = w;
					break;
				}
			}
		}else{
			for (int i = 0; i < numOfNode; i++){
				if(B[1][i] == -1){
					B[1][i] = w;
					break;
				}
			}
		}
	}
}
void sssp_m (int **graphmatric, int comm_sz, int my_rank ){
	relax(0,0);

	int work_per_processor = numOfNode / comm_sz;
	if (work_per_processor * comm_sz < numOfNode) work_per_processor++;

	// initialize 
	int request_light_local[numOfNode];
	int request_light_global[numOfNode];
	int request_heavy_local[numOfNode];
	int request_heavy_global[numOfNode];
	int localIndex;
	int index;
	memset(&request_light_local,999999,sizeof(int) * numOfNode);
	memset(&request_light_global,999999,sizeof(int) * numOfNode);
	memset(&request_heavy_local,999999,sizeof(int) * numOfNode);
	memset(&request_heavy_global,999999,sizeof(int) * numOfNode);

	for (int i = 0; i < 2; i++){
		int s[numOfNode];
		memset(&s, -1, sizeof(int) * numOfNode);
		int index_s = 0;
		while (B[i][0] != -1){
			for (int j = 0; j < work_per_processor; j++){
				localIndex = my_rank * work_per_processor + j;
				if (B[i][localIndex] == -1 || localIndex >= numOfNode) break;
				index = B[i][localIndex];
				// printf("%d from %d\n",index,my_rank );
				B[i][localIndex] = -1;
				s[index_s++] = index;
				for (int k = 0; k < numOfNode; k++){
					if (graphmatric[index][k] != 0 && (graphmatric[index][k] - 1) / delta == 0){
						if (request_light_local[k] == 0 || request_light_local[k] > tent[index] + graphmatric[index][k])
							request_light_local[k] = tent[index] + graphmatric[index][k];
					}else if (graphmatric[index][k] != 0){
						if (request_heavy_local[k] == 0 || request_heavy_local[k] > tent[index] + graphmatric[index][k])
							request_heavy_local[k] = tent[index] + graphmatric[index][k];
					}
				}
			}
			MPI_Allreduce(&request_light_local,&request_light_global,numOfNode,MPI_INT,MPI_MIN,MPI_COMM_WORLD);
			MPI_Allreduce(&request_heavy_local,&request_heavy_global,numOfNode,MPI_INT,MPI_MIN,MPI_COMM_WORLD);
			memset(B[i], -1, sizeof(int) * numOfNode);
			for (int j = 0; j < numOfNode; j++){
				if (request_light_global[j] != 999999){
					relax(j,request_light_global[j]);
					request_heavy_global[j] = 999999;
				}
			}
			// break;
		}
		for (int j = 0; j < numOfNode; j++){
			if (request_heavy_global[j] != 999999){
				relax(j, request_heavy_global[j]);
				request_heavy_global[j] = 999999;
			}
		}
		// break;
	}
	if (my_rank == 0){
		for (int i = 0; i < numOfNode; ++i)
			printf("%d ",tent[i] );
		printf("\n");
	}
	
}

void sssp_a (Node *head, int comm_sz, int my_rank ){
	relax(0,0);
	int work_per_processor = numOfNode / comm_sz;
	if (work_per_processor * comm_sz < numOfNode) work_per_processor++;

	// initialize 
	int request_light_local[numOfNode];
	int request_light_global[numOfNode];
	int request_heavy_local[numOfNode];
	int request_heavy_global[numOfNode];
	int localIndex;
	int index;
	int nodetoindex;
	int weight;
	memset(&request_light_local,999999,sizeof(int) * numOfNode);
	memset(&request_light_global,999999,sizeof(int) * numOfNode);
	memset(&request_heavy_local,999999,sizeof(int) * numOfNode);
	memset(&request_heavy_global,999999,sizeof(int) * numOfNode);

	for (int i = 0; i < 2; i++){
		int s[numOfNode];
		memset(&s, -1, sizeof(int) * numOfNode);
		int index_s = 0;
		while (B[i][0] != -1){
			for (int j = 0; j < work_per_processor; j++){
				localIndex = my_rank * work_per_processor + j;
				if (B[i][localIndex] == -1 || localIndex >= numOfNode) break;
				int index = B[i][localIndex];
				// printf("%d from %d\n",index,my_rank );
				B[i][localIndex] = -1;
				s[index_s++] = index;
				Node ntmp = (Node)head[index];
				Edge *etmp = (Edge *)ntmp.edgehead;
				while(etmp != NULL){
					nodetoindex = etmp->nodeto;
					weight = etmp->weight;
					if ((weight-1)/delta == 0) {
						if (request_light_local[nodetoindex] == 0 || request_light_local[nodetoindex] > tent[index] + weight)
							request_light_local[nodetoindex] = tent[index] + weight;
					}else{
						if (request_heavy_local[nodetoindex] == 0 || request_heavy_local[nodetoindex] > tent[index] + weight)
							request_heavy_local[nodetoindex] = tent[index] + weight;
					}  
					etmp = etmp->nextedge;
				}
			}

			MPI_Allreduce(&request_light_local,&request_light_global,numOfNode,MPI_INT,MPI_MIN,MPI_COMM_WORLD);
			MPI_Allreduce(&request_heavy_local,&request_heavy_global,numOfNode,MPI_INT,MPI_MIN,MPI_COMM_WORLD);
			memset(B[i], -1, sizeof(int) * numOfNode);
			for (int j = 0; j < numOfNode; j++){
				if (request_light_global[j] != 0){
					relax(j,request_light_global[j]);
					request_light_global[j] = 999999;
				}
			}
			// break;
		}
		for (int j = 0; j < numOfNode; j++){
			if (request_heavy_global[j] != 0){
				relax(j, request_heavy_global[j]);
				request_heavy_global[j] = 999999;
			}
		}
		// break;
	}
	if (my_rank == 0){
		for (int i = 0; i < numOfNode; ++i)
			printf("%d ",tent[i] );
		printf("\n");
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
			creatMatric(nameOfFile,numOfNode,graphmatric);
			initialize(graphmatric);
			sssp_m(graphmatric,comm_sz,my_rank);
			// dijkstra(graphmatric, numOfNode);
			free(graphmatric);
		}else{
			result = strcmp(mode_adjlist,mode);
			if (result == 0){
				Node nodelist[numOfNode]; // number of node array
				createAdjList(nameOfFile,numOfNode,(Node *)&nodelist);
				// loop to go through all list 
				initialize_a((Node *)&nodelist);
				printf("%d\n", delta);
				sssp_a((Node *)&nodelist,comm_sz,my_rank);
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