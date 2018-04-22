#include "loadgraph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h> 
// global 
int *visited;
int *worklist;
int numOfNode;
int numOfThread;
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
void initialize(){
	visited = malloc(numOfNode * sizeof(int));
	worklist = malloc(numOfNode * sizeof(int));
	for (int i = 0; i < numOfNode; i++){
		visited[i] = -1;
		worklist[i] = -1;
	}
}
int checkWorkList(){
	if (worklist[0] == -1) return 0;
	else return 1;
}
void bfs_m(int **graphmatric, int comm_sz, int my_rank){
	// printf("%d\n",checkWorkList() );
	int startpoint = 1;
	int nextlevel_global[numOfNode *comm_sz];
	memset(&nextlevel_global, -1, sizeof(int) * numOfNode * comm_sz);
	// printf("%d\n", checkWorkList() );
	while (checkWorkList() == 1 ){
		int num_per_processor = startpoint / comm_sz; // work for every processor
		if (num_per_processor * comm_sz < startpoint) num_per_processor++; 
		int nextlevel[numOfNode];
		memset(&nextlevel, -1, sizeof(int) * numOfNode);
		for(int i = 0; i < num_per_processor; i++){
			int realindex = num_per_processor * my_rank + i;
			if (realindex >= startpoint) break;
			int currentIndex = worklist[realindex];
			for (int j = 0; j < numOfNode; j++){
				if (graphmatric[currentIndex][j] != 0 && visited[j] == -1){
					for (int k = 0; k < numOfNode; k++){
						if (nextlevel[k] == -1){
							nextlevel[k] = j;
							break;
						}else if (nextlevel[k] == j){
							break;
						}
					}
				}
			}
		}
		MPI_Allgather(&nextlevel,numOfNode,MPI_INT,&nextlevel_global,numOfNode,MPI_INT,MPI_COMM_WORLD); 
		memset(worklist, -1, sizeof(int) * startpoint);
		startpoint = 0;
		for (int i = 0; i < comm_sz; i++){
			for( int j = 0; j < numOfNode; j++){
				int currentIndex = i * numOfNode + j;
				if (nextlevel_global[currentIndex] == -1) break;
				else{
					int nexttmp = nextlevel_global[currentIndex];
					if (visited[nexttmp] == -1){
						visited[nexttmp] = 0;
						worklist[startpoint++] = nexttmp;
					}
				}
			}
		}
		// if (my_rank == 0){
			for (int i = 0; worklist[i] != -1; i++){
				printf("%d ",worklist[i] );
			}
			printf(" from %d\n", my_rank);
		// }
	}
}
void bfs_a(Node *head,int comm_sz, int my_rank){
	// printf("%d\n",checkWorkList() );
	int startpoint = 1;
	int nextlevel_global[numOfNode * comm_sz];
	memset(&nextlevel_global, -1, sizeof(int) * numOfNode * comm_sz);
	while (checkWorkList() == 1){
		int num_per_processor = startpoint / comm_sz; // work for every processor
		if (num_per_processor * comm_sz < startpoint) num_per_processor++; 
		int nextlevel[numOfNode];
		memset(&nextlevel, -1, sizeof(int) * numOfNode);

		for(int i = 0; i < num_per_processor; i++){
			int realindex = num_per_processor * my_rank + i;
			if (realindex >= startpoint) break;
			int currentIndex = worklist[realindex];
			// sprintf("%d from %d\n",currentIndex,my_rank );
			Node ntmp = (Node)head[i];
			Edge *etmp = (Edge *)ntmp.edgehead;
			while(etmp != NULL){
				int j = etmp->nodeto;
				if (visited[j] == -1){
					for (int k = 0; k < numOfNode; k++){
						if (nextlevel[k] == -1){
							nextlevel[k] = j;
							break;
						}else if (nextlevel[k] == j){
							break;
						}
					}
				}
				etmp = etmp ->nextedge;
			}
		}
		MPI_Allgather(&nextlevel,numOfNode,MPI_INT,&nextlevel_global,numOfNode,MPI_INT,MPI_COMM_WORLD); 
		memset(worklist, -1, sizeof(int) * startpoint);
		startpoint = 0;
		for (int i = 0; i < comm_sz; i++){
			for( int j = 0; j < numOfNode; j++){
				int currentIndex = i * numOfNode + j;
				if (nextlevel_global[currentIndex] == -1) break;
				else{
					int nexttmp = nextlevel_global[currentIndex];
					if (visited[nexttmp] == -1){
						visited[nexttmp] = 0;
						worklist[startpoint++] = nexttmp;
					}
				}
			}
		}
		for (int i = 0; worklist[i] != -1; i++){
			printf("%d ",worklist[i] );
		}
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
			printf("Illegal number of node %s\n",argv[1] );
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
			// initialize 
			initialize();
			worklist[0] = 0;
			visited[0] = 0;
			bfs_m(graphmatric,comm_sz,my_rank);
			free(graphmatric);
		}else{
			result = strcmp(mode_adjlist,mode);
			if (result == 0){
				Node nodelist[numOfNode]; // number of node array
				createAdjList(nameOfFile,numOfNode,(Node *)&nodelist);
				// loop to go through all list 
				/*for (int i = 0; i < numOfNode; i++){
					Node ntmp = (Node)nodelist[i];
					Edge *etmp = (Edge *)ntmp.edgehead;
					printf("Node num: %d\n", ntmp.nodeNum);
					while (etmp != NULL){
						printf("[from: %d to: %d weight: %d \t",etmp->nodefrom,etmp->nodeto,etmp->weight );
						etmp = etmp->nextedge;
					}
					printf("\n");
				}*/
				initialize();
				worklist[0] = 0;
				visited[0] = 0;
				bfs_a((Node *)&nodelist,comm_sz,my_rank);
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