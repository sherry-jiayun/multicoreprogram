#include "loadgraph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
			int **graphmatric;
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
			free(graphmatric);
		}else{
			result = strcmp(mode_adjlist,mode);
			if (result == 0){
				Node nodelist[numOfNode];
				createAdjList(nameOfFile,numOfNode,(Node *)&nodelist);
				for (int i = 0; i < 1; i++){
				Node ntmp = (Node)nodelist[i];
				Edge *etmp = (Edge *)ntmp.edgehead;
				printf("Node num: %d\n", ntmp.nodeNum);
				// printf("[from: %d to: %d weight: %d \n",ntmp.edgehead->nodefrom,ntmp.edgehead->nodeto,ntmp.edgehead->weight );
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