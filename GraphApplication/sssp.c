#include "loadgraph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

int shortestvertex(int *distance, int *connection, int numOfNode){
	int index = 0;
	int min = INFINITY;
	for(int i = 0; i < numOfNode; i++){
		if(connection[i] == 0 && distance[i] < min){
			min = distance[i];
			index = i;
		}
	}
	return index;
}

void dijkstra(int **graphmatric, int numOfNode){
    //an array to set original distance from single scource to every vertics
    int *distance;
    distance = malloc(numOfNode * sizeof(int));
    distance[0] = 0;
    for(int i = 1; i < numOfNode; i++){
    	distance[i] = INFINITY;
    }

    int *connection;
    connection = malloc(numOfNode * sizeof(int));
    for(int i = 0; i < numOfNode; i ++){
    	connection[i] = 0;
    }

    for(int i = 0; i < numOfNode; i++){
    	int nextV = shortestvertex(distance, connection, numOfNode);
    	connection[nextV] = 1;

    	for(int j = 0; j < numOfNode; j++){
    		if(connection[j] == 0 && graphmatric[i][j] > 0 && 
    			distance[i] + graphmatric[i][j] < distance[j]){
    			distance[j] = distance[i] + graphmatric[i][j];
    		}
    	}
    }

    // print the distances
    for(int i = 0; i < numOfNode; i++){
    	printf("%d\n", distance[i]);
    }
    free(distance);
    free(connection);
}

int main(int argc, char *argv[])    
{
	int numOfNode;
	char *nameOfFile;
	int index = 0;
	char *mode;
	char mode_matrix[2];
	char mode_adjlist[2];
	strcpy(mode_matrix,"m");
	strcpy(mode_adjlist,"l");
	if(argc == 4){
		numOfNode = getInt(argv[1]);
		if (numOfNode == -1){
		printf("Illegal number of city %s\n",argv[1] );
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
			for (int i = 0; i < numOfNode; i++){
				printf("Node No.%d\t",i);
				for (int j = 0; j < numOfNode; j++){
					printf("%d  ",graphmatric[i][j] );
				}
				printf("\n");
			}
			dijkstra(graphmatric, numOfNode);
			free(graphmatric);
		}else{
			result = strcmp(mode_adjlist,mode);
			if (result == 0){
				Node nodelist[numOfNode]; // number of node array
				createAdjList(nameOfFile,numOfNode,(Node *)&nodelist);
				// loop to go through all list 
				for (int i = 0; i < numOfNode; i++){
					Node ntmp = (Node)nodelist[i];
					Edge *etmp = (Edge *)ntmp.edgehead;
					printf("Node num: %d\n", ntmp.nodeNum);
					while (etmp != NULL){
						printf("[from: %d to: %d weight: %d \t",etmp->nodefrom,etmp->nodeto,etmp->weight );
						etmp = etmp->nextedge;
					}
					printf("\n");
				}
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