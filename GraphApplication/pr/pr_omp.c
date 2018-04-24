#include "loadgraph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

/*** global variable ***/

int numOfNode;
int numOfThread;
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
int pagerankchack(float *prold, float *prnew, int numOfNode){
	float sum = 0;
	for (int i = 0; i < numOfNode; i++){
		float sumtmp = prold[i] - prnew[i];
		if (sumtmp < 0){
			sum -= sumtmp;
		}else{
			sum += sumtmp;
		}
		prold[i] = prnew[i];
	}
	if (sum_global - sum < 0.000001 && sum_global - sum > -0.000001) return 0;
	sum_global = sum;
	return 1;
}
void pagerankm(int **graphmatric){
	// iniitalize pagerank
	float *pagerankinitial;
	int *inouttable;
	int iter = 0;
	pagerankinitial = (float *)malloc(numOfNode * sizeof(float));
	inouttable = malloc(numOfNode * sizeof(int));
	float initializeNum = 1/(float)numOfNode;
	#pragma omp parallel
	{
		#pragma omp for
		for (int i = 0; i < numOfNode; i++){
			pagerankinitial[i] = initializeNum;
			inouttable[i] = 0;
			for (int j = 0; j < numOfNode; j++){
				if (graphmatric[i][j] != 0){
					inouttable[i] += 1;
				} 
			}
		}
	}
	float *prnew;
	prnew = (float *)malloc(numOfNode * sizeof(float));
	int checkResult = 1;
	while (checkResult == 1){
		iter++;
		#pragma omp parallel
		{	
			#pragma omp for 
			for (int i = 0; i < numOfNode; i++){
				prnew[i] = 0;
				for (int j = 0; j <numOfNode; j++){
					if (graphmatric[j][i] != 0){
						prnew[i] += pagerankinitial[j]/inouttable[j];
					}
				}
			}

		}
		checkResult = pagerankchack(pagerankinitial,prnew,numOfNode);
	}
	/*for (int i = 0; i < numOfNode; i++){
		printf("Node No.%d now: %f\n",i,pagerankinitial[i]);
	}*/
	printf("Iteration: %d\n",iter );
}
void pageranka(Node *head){

	// initialize pagerank
	Node a_node[numOfNode]; // reorganize adj list 
	float *pagerankinitial; // initialize pr for every node
	int *inouttable; // calculate inout edge num for each node 
	pagerankinitial = (float *)malloc(numOfNode * sizeof(float));
	int iter = 0;
	inouttable = malloc(numOfNode * sizeof(int));

	float initializeNum = 1/(float)numOfNode;
	#pragma omp for
	for (int i = 0; i < numOfNode; i++){
		a_node[i].nodeNum = i;
		a_node[i].edgehead = NULL;
	}
	#pragma omp for
	for (int i = 0; i < numOfNode; i++){
		pagerankinitial[i] = initializeNum;
		inouttable[i] = 0;
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
			inouttable[i] += 1;
			etmp = etmp ->nextedge;
		}
		// printf("Node No.%d out route number %d\n",i,inouttable[i] );
	}
	
	float *prnew;
	prnew = (float *)malloc(numOfNode * sizeof(float));
	int checkResult = 1;
	while (checkResult == 1){
		iter++;
		#pragma omp for
		for (int i = 0; i < numOfNode; i++){
			prnew[i] = 0;
			Node ntmp = (Node)a_node[i];
			Edge *etmp = (Edge *)ntmp.edgehead;
			while(etmp != NULL){
				int fromNode = etmp->nodefrom;
				// printf("%d\n",fromNode );
				prnew[i] += pagerankinitial[fromNode]/inouttable[fromNode];
				etmp = etmp ->nextedge;
			}
			// printf("Node No.%d old: %f new: %f\n",i,pagerankinitial[i],prnew[i] );
		}
		checkResult = pagerankchack(pagerankinitial,prnew,numOfNode);
	}
	/*for (int i = 0; i < numOfNode; i++){
		printf("Node No.%d now: %f\n",i,pagerankinitial[i]);
	}*/
	printf("Iteration: %d\n",iter );

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
		omp_set_num_threads(numOfThread);
		if (result == 0){
			int **graphmatric; // numOfNode * numOfNode matrix 
			graphmatric = malloc(numOfNode * sizeof *graphmatric);
			for (;index < numOfNode;index++){
				graphmatric[index] = malloc(numOfNode * sizeof(int));
			}
			creatMatric(nameOfFile,numOfNode,graphmatric);
			pagerankm(graphmatric);
			free(graphmatric);
		}else{
			result = strcmp(mode_adjlist,mode);
			if (result == 0){
				Node nodelist[numOfNode]; // number of node array
				createAdjList(nameOfFile,numOfNode,(Node *)&nodelist);
				pageranka((Node *)&nodelist);
				
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