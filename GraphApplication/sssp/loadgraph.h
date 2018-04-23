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

