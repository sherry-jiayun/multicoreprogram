#include "loadgraph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


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
void sssp_m (int **graphmatric ){
	relax(0,0);
	int request_light[numOfNode];
	int request_heavy[numOfNode];
	memset(&request_light,0,sizeof(int) * numOfNode);
	memset(&request_heavy,0,sizeof(int) * numOfNode);
	for (int i = 0; i < 2; i++){
		int s[numOfNode];
		memset(&s, -1, sizeof(int) * numOfNode);
		int index_s = 0;
		while (B[i][0] != -1){
			int loopnumber = 0;
			for (int j = 0; j < numOfNode; j++){
				if (B[i][j] == -1) break;
				loopnumber++;
				int index = B[i][j];
				B[i][j] = -1;
				s[index_s++] = index;
				for (int k = 0; k < numOfNode; k++){
					if (graphmatric[index][k] != 0 && (graphmatric[index][k] - 1) / delta == 0){
						if (request_light[k] == 0 || request_light[k] > tent[index] + graphmatric[index][k])
							request_light[k] = tent[index] + graphmatric[index][k];
					}else if (graphmatric[index][k] != 0){
						if (request_heavy[k] == 0 || request_heavy[k] > tent[index] + graphmatric[index][k])
							request_heavy[k] = tent[index] + graphmatric[index][k];
					}
				}
			}
			for (int j = 0; j < numOfNode; j++){
				if (request_light[j] != 0){
					relax(j,request_light[j]);
					request_light[j] = 0;
				}
			}
			// break;
		}
		for (int j = 0; j < numOfNode; j++){
			if (request_heavy[j] != 0){
				relax(j, request_heavy[j]);
				request_heavy[j] = 0;
			}
		}
		// break;
	}
	/*for (int i = 0; i < numOfNode; ++i)
		printf("%d ",tent[i] );
	printf("\n");*/
	
}

void sssp_a (Node *head ){
	relax(0,0);
	int request_light[numOfNode];
	int request_heavy[numOfNode];
	int nodetoindex;
	int weight;
	memset(&request_light,0,sizeof(int) * numOfNode);
	memset(&request_heavy,0,sizeof(int) * numOfNode);
	for (int i = 0; i < 2; i++){
		int s[numOfNode];
		memset(&s, -1, sizeof(int) * numOfNode);
		int index_s = 0;
		while (B[i][0] != -1){
			for (int j = 0; j < numOfNode; j++){
				if (B[i][j] == -1) break;
				int index = B[i][j];
				B[i][j] = -1;
				s[index_s++] = index;
				Node ntmp = (Node)head[index];
				Edge *etmp = (Edge *)ntmp.edgehead;
				while(etmp != NULL){
					nodetoindex = etmp->nodeto;
					weight = etmp->weight;
					if ((weight-1)/delta == 0) {
						if (request_light[nodetoindex] == 0 || request_light[nodetoindex] > tent[index] + weight)
							request_light[nodetoindex] = tent[index] + weight;
					}else{
						if (request_heavy[nodetoindex] == 0 || request_heavy[nodetoindex] > tent[index] + weight)
							request_heavy[nodetoindex] = tent[index] + weight;
					} 
					etmp = etmp->nextedge;
				}
			}
			for (int j = 0; j < numOfNode; j++){
				if (request_light[j] != 0){
					relax(j,request_light[j]);
					request_light[j] = 0;
				}
			}
			// break;
		}
		for (int j = 0; j < numOfNode; j++){
			if (request_heavy[j] != 0){
				relax(j, request_heavy[j]);
				request_heavy[j] = 0;
			}
		}
		// break;
	}
	/*for (int i = 0; i < numOfNode; ++i)
		printf("%d ",tent[i] );
	printf("\n");*/
	
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
			initialize(graphmatric);
			sssp_m(graphmatric);
			// dijkstra(graphmatric, numOfNode);
			free(graphmatric);
		}else{
			result = strcmp(mode_adjlist,mode);
			if (result == 0){
				Node nodelist[numOfNode]; // number of node array
				createAdjList(nameOfFile,numOfNode,(Node *)&nodelist);
				// loop to go through all list 
				initialize_a((Node *)&nodelist);
				sssp_a((Node *)&nodelist);
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