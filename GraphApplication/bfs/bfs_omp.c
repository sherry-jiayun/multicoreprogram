#include "loadgraph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

// global 
int *visited;
int *worklist;
int numOfNode;
int numOfThread;
/***
在头文件里引用 "loadgraph.h"
用下面这一段当作main函数
运行的时候使用 ./可执行文件 node数目 文件名 模式(m for matrix l for adj list) numOfThread
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
void bfs_m(int **graphmatric){
	// printf("%d\n",checkWorkList() );
	int startpoint = 1;
	omp_set_num_threads(numOfThread);
	while (checkWorkList() == 1){
		/* global variable */
		int nextlever_global[numOfNode];
		memset(&nextlever_global, -1, sizeof(int) * numOfNode);
		/* local variable */ 
		#pragma omp parallel
		{
			int nextlever[numOfNode];
			memset(&nextlever, -1, sizeof(int) * numOfNode);
			#pragma omp for
			for(int i = 0; i < startpoint; i++){
				
				int currentIndex = worklist[i];
				worklist[i] = -1;
				for (int j = 0; j < numOfNode; j++){
					if (graphmatric[currentIndex][j] != 0 && visited[j] == -1){
						for (int k = 0; k < numOfNode; k++){
							if (nextlever[k] == -1){
								nextlever[k] = j;
								break;
							}else if (nextlever[k] == j){
								break;
							}
						}
					}
				}
			}
		
			/* critical function */
			#pragma omp critical
			for (int i = 0; i < numOfNode; i++){
				if (nextlever[i] == -1){
					break;
				}else{
					int nexttmp = nextlever[i];
					for (int j = 0; j < numOfNode;j++){
						if (nextlever_global[j] == -1){
							nextlever_global[j] = nexttmp;
							break;
						}else if (nextlever_global[j] == nexttmp){
							break;
						}
					}
				}
			}
		}
		/* end of para */
		startpoint = 0;
		for (int j = 0; j < numOfNode; j++){
			if (nextlever_global[j] == -1){
				break;
			}else{
				int nltmp = nextlever_global[j];
				worklist[startpoint++] = nltmp;
				visited[nltmp] = 0;
			}
		}
		/*for (int i = 0; worklist[i] != -1; i++){
			printf("%d ",worklist[i] );
		}
		printf("\n");*/
	}
}

void bfs_a(Node *head){
	// printf("%d\n",checkWorkList() );
	int startpoint = 1;
	omp_set_num_threads(numOfThread);
	while (checkWorkList() == 1){
		int nextlever_global[numOfNode];
		memset(&nextlever_global, -1, sizeof(int) * numOfNode);
		#pragma omp parallel
		{
			int nextlever[numOfNode];
			memset(&nextlever, -1, sizeof(int) * numOfNode);
			#pragma omp for
			for(int i = 0; i < startpoint; i++){
				int currentIndex = worklist[i];
				worklist[i] = -1;
				Node ntmp = (Node)head[i];
				Edge *etmp = (Edge *)ntmp.edgehead;
				while(etmp != NULL){
					int j = etmp->nodeto;
					if (visited[j] == -1){
						for (int k = 0; k < numOfNode; k++){
							if (nextlever[k] == -1){
								nextlever[k] = j;
								break;
							}else if (nextlever[k] == j){
								break;
							}
						}
					}
					etmp = etmp ->nextedge;
				}
			}
			#pragma omp critical
			for (int i = 0; i < numOfNode; i++){
				if (nextlever[i] == -1){
					break;
				}else{
					int nexttmp = nextlever[i];
					for (int j = 0; j < numOfNode;j++){
						if (nextlever_global[j] == -1){
							nextlever_global[j] = nexttmp;
							break;
						}else if (nextlever_global[j] == nexttmp){
							break;
						}
					}
				}
			}
		}
		startpoint = 0;
		for (int j = 0; j < numOfNode; j++){
			if (nextlever_global[j] == -1){
				break;
			}else{
				int nltmp = nextlever_global[j];
				worklist[startpoint++] = nltmp;
				visited[nltmp] = 0;
			}
		}
		/*for (int i = 0; worklist[i] != -1; i++){
			printf("%d ",worklist[i] );
		}
		printf("\n");*/
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
	if(argc == 5){
		numOfNode = getInt(argv[1]);
		if (numOfNode == -1){
		printf("Illegal number of node %s\n",argv[1] );
		return 0;
		}
		nameOfFile = argv[2];
		mode = argv[3];
		numOfThread = getInt(argv[4]);
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
			bfs_m(graphmatric);
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
				bfs_a((Node *)&nodelist);
			}else{
				printf("Illegal mode!\n");
				return 0;
			}
		}
	}else{
		printf("Not have enough para.\n");
		return 0;
	}
	return 0;
}