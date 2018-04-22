#include "loadgraph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

/* global **/
int numOfNode;
int numOfThread;

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

int shortestvertex(int *distance, int *connection){
	int index = 0;
	int min = 999999;
	for(int i = 0; i < numOfNode; i++){
		if (distance[i] < min && connection[i] == 0){
			min = distance[i];
			index = i;
		}
	}
	return index;
}

void dijkstra_m(int **graphmatric){
    //an array to set original distance from single scource to every vertics
    int *distance;
    int *connection;
    omp_set_num_threads(numOfThread);
    distance = malloc(numOfNode * sizeof(int));
    connection = malloc(numOfNode * sizeof(int));
    distance[0] = 0;
    #pragma omp parallel
    {
    	#pragma omp for
		for(int i = 1; i < numOfNode; i++){
			distance[i] = 999999;
			connection[i] = 0;
		}
    }
   
    for(int i = 0; i < numOfNode; i++){
    	int nextV = shortestvertex(distance,connection);
    	connection[nextV] = 1;
    	#pragma omp parallel
    	{
    		#pragma omp for
    		for(int j = 0; j < numOfNode; j++){
	    		if( graphmatric[nextV][j] > 0 && (distance[nextV] + graphmatric[nextV][j]) < distance[j]){
	    			distance[j] = distance[nextV] + graphmatric[nextV][j];
	    		}
	    	}
    	}
    }
    for (int j = 0; j < numOfNode; j++){
		printf("%d ",distance[j] );
	}
	printf("\n");
    free(distance);
    free(connection);
}
void dijkstra_a(Node *head){
	int *distance;
	int *connection;
    distance = malloc(numOfNode * sizeof(int));
    connection = malloc(numOfNode * sizeof(int));
    distance[0] = 0;
    #pragma omp parallel
    {
		for(int i = 1; i < numOfNode; i++){
			distance[i] = 999999;
			connection[i] = 0;
		}
    }

    for(int i = 0; i < numOfNode; i++){
    	int nextV = shortestvertex(distance, connection);
    	connection[nextV] = 1;
    	Node ntmp = (Node)head[nextV];
		Edge *etmp = (Edge *)ntmp.edgehead;
		while(etmp != NULL){
			int j = etmp->nodeto;
			if (distance[nextV] + etmp->weight < distance[j]){
				distance[j] = distance[nextV] + etmp->weight;
			}
			etmp = etmp->nextedge;
		}
    }

    // print the distances
    for(int i = 0; i < numOfNode; i++){
    	printf("%d ", distance[i]);
    }
    printf("\n");
    free(distance);
    free(connection);
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
		printf("Illegal number of city %s\n",argv[1] );
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
			/*for (int i = 0; i < numOfNode; i++){
				printf("Node No.%d\t",i);
				for (int j = 0; j < numOfNode; j++){
					printf("%d  ",graphmatric[i][j] );
				}
				printf("\n");
			}*/
			dijkstra_m(graphmatric);
			free(graphmatric);
		}else{
			result = strcmp(mode_adjlist,mode);
			if (result == 0){
				Node nodelist[numOfNode]; // number of node array
				createAdjList(nameOfFile,numOfNode,(Node *)&nodelist);
				// loop to go through all list 
				dijkstra_a((Node *)&nodelist);
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