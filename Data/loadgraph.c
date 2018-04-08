#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int getInt(char *intStr){
  // printf("%s with length %lu\n",intStr,strlen(intStr));
  int lengthOfStr = strlen(intStr);
  int index = 0;
  for (; index < lengthOfStr; index++ ){
    char c = intStr[index];
    if (c < '0' || c > '9'){
      return -1;
    }
  }
  int num = 0;
  num = atoi(intStr);
  return num;
}
void createAdjList(char *fileName, int numOfNode, Node *nodelist){
  FILE *fp;
  char buff[100];
  fp = fopen(fileName,"r");
  int nodeNum = 0;
  int linkNum = 0;
  
  while((fscanf(fp,"%s",buff)) == 1){
    int weightNum = getInt(buff);
    if (weightNum != -1){
      if (linkNum == 0){
        nodelist[nodeNum].nodeNum = nodeNum;
        nodelist[nodeNum].edgehead = NULL;
      }
      if (weightNum != 0){
        Edge *edgecurrent;
        edgecurrent = malloc(sizeof(Edge));
        edgecurrent->nodefrom = nodeNum;
        edgecurrent->nodeto = linkNum;
        edgecurrent->weight = weightNum;
        // memcpy(edgecurrent.nextedge,nodelist[nodeNum].edgehead,sizeof (*Edge))
        edgecurrent->nextedge = nodelist[nodeNum].edgehead;
        nodelist[nodeNum].edgehead = edgecurrent;
      }
      if (linkNum == numOfNode - 1){
        linkNum = 0;
        nodeNum++;
      }else{
        linkNum++;
      }
    }
  }
  fclose(fp);
  return;
}

void creatMatric(char *fileName,int numOfNode,int **graphmatric){

  FILE *fp;
  char buff[100];

  fp = fopen(fileName,"r");
  int nodeNum = 0;
  int linkNum = 0;

  while((fscanf(fp,"%s",buff)) == 1){
    int weightNum = getInt(buff);
    if (weightNum != -1){
      graphmatric[nodeNum][linkNum] = weightNum;
      if (linkNum == numOfNode-1){
        linkNum = 0;
        nodeNum++;
      }else{
        linkNum++;
      }
    }else{
      printf("Illegal weight number.\n");
      exit(0);
    }
  }
  fclose(fp);
  return;
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