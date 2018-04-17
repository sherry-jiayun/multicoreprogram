#include "loadgraph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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
void connectedComponents_m(int **graphmatric, int numOfNode);
void connectedComponents_a(Node *nodelist, int numOfNode);

// A structure to represent a queue
struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int* array;
    Node* nodes;
};
 
// function to create a queue of given capacity. 
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0; 
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array = (int*) malloc(queue->capacity * sizeof(int));
    queue->nodes = (Node*) malloc(queue->capacity * sizeof(Node));
    return queue;
}
 
// Queue is full when size becomes equal to the capacity 
int isFull(struct Queue* queue)
{  return (queue->size == queue->capacity);  }
 
// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{  return (queue->size == 0); }
 
// Function to add an item to the queue.  
// It changes rear and size
void enqueue(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    printf("%d enqueued to queue\n", item);
}

//Function to add a node to the queue
void enqueueNode(struct Queue* queue, Node item) {
	if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->nodes[queue->rear] = item;
    queue->size = queue->size + 1;
    //printf("%d enqueued to queue\n", item);
}
 
// Function to remove an item from queue. 
// It changes front and size
int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

//Function to remove a node from queue
Node* dequeueNode(struct Queue* queue)
{
    if (isEmpty(queue)) {
        printf("Error: queue is empty when try to dequeue");
        return NULL;
    }
    Node node = queue->nodes[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    Node* item = &node;
    return item;
}
 
// Function to get front of queue
int front(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}
 
// Function to get rear of queue
int rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

int main(int argc, char *argv[]) {
	char *nameOfFile;
	int index = 0;
	char *mode;
	char mode_matrix[2];
	char mode_adjlist[2];
	int numOfNode;
	strcpy(mode_matrix, "m");
	strcpy(mode_adjlist, "l");
	if(argc == 4){
		numOfNode = getInt(argv[1]);
		if (numOfNode == -1){
			printf("Illegal number of city %s\n", argv[1] );
			return 0;
		}
		nameOfFile = argv[2];
		mode = argv[3];
		int result = strcmp(mode_matrix, mode);
		if (result == 0){
			int **graphmatric; // numOfNode * numOfNode matrix 
			graphmatric = malloc(numOfNode * sizeof *graphmatric);
			for (;index < numOfNode; index++) {
				graphmatric[index] = malloc(numOfNode * sizeof(int));
			}
			creatMatric(nameOfFile,numOfNode,graphmatric);
			for (int i = 0; i < numOfNode; i++) {
				printf("Node No.%d\t",i);
				for (int j = 0; j < numOfNode; j++) {
					printf("%d  ",graphmatric[i][j]);
				}
				printf("\n");
			}
			//connectedComponents_m(graphmatric, numOfNode);
			free(graphmatric);
		} else {
			result = strcmp(mode_adjlist, mode);
			if (result == 0) {
				Node nodelist[numOfNode]; // number of node array
				createAdjList(nameOfFile, numOfNode, (Node *) &nodelist);
				// loop to go through all list 
				for (int i = 0; i < numOfNode; i++) {
					Node ntmp = (Node)nodelist[i];
					Edge *etmp = (Edge *)ntmp.edgehead;
					printf("Node num: %d\n", ntmp.nodeNum);
					while (etmp != NULL) {
						printf("[from: %d to: %d weight: %d \t", etmp->nodefrom,etmp->nodeto,etmp->weight );
						etmp = etmp->nextedge;
					}
					printf("\n");
				}
			} else {
				printf("Illegal mode!\n");
				return 0;
			}
		}
	} else {
		printf("Not have enough para.\n");
		return 0;
	}
	return 0;
}

void connectedComponents_m(int **graphmatric, int numOfNode) {
	struct Queue* queue = createQueue(numOfNode);
	int groups[numOfNode][numOfNode];
	int groupIndex[numOfNode];
	int visited[numOfNode];
	int groupNum;
	int i;
	int j;

	//initialization
	for (i = 0; i < numOfNode; i++) {
		visited[i] = 0;
		groupIndex[i] = -1;
	}
	for (i = 0; i < numOfNode; i++) {
		for (j = 0; j < numOfNode; j++) {
			groups[i][j] = -1;
		}
	}
	groupNum = 0;
	groups[groupNum][0] = 0;
	groupIndex[0] = groupNum;

	//BFS
	enqueue(queue, 0);
	visited[0] = 1;
	while (!isEmpty(queue)) {
		int current = dequeue(queue);
		for (i = 0; i < numOfNode; i++) {
			if (i != current && graphmatric[current][i] != 0 && visited[i] == 0) {
				enqueue(queue, i);
				visited[i] = 1;
				if (groupIndex[current] != -1) {
					int index = groupIndex[current];
					int next = 0;
					while (groups[index][next] != -1) {
						next = next + 1;
					}
					groups[index][next] = i;
					groupIndex[i] =index;
				} else {
					groupNum = groupNum + 1;
					groups[groupNum][0] = current;
					groupIndex[current] = groupNum;
					groups[groupNum][1] = i;
					groupIndex[i] = groupNum;
				}
			}
		}
	}
}

void connectedComponents_a(Node *nodelist, int numOfNode) {
	struct Queue* queue = createQueue(numOfNode);
	int groups[numOfNode][numOfNode];
	int groupIndex[numOfNode];
	int visited[numOfNode];
	int groupNum;
	int i;
	int j;

	//initialization
	for (i = 0; i < numOfNode; i++) {
		visited[i] = 0;
		groupIndex[i] = -1;
	}
	for (i = 0; i < numOfNode; i++) {
		for (j = 0; j < numOfNode; j++) {
			groups[i][j] = -1;
		}
	}

	Node start = (Node) nodelist[0];
	groupNum = 0;
	groups[groupNum][0] = start.nodeNum;
	groupIndex[start.nodeNum] = groupNum;

	//BFS
	enqueueNode(queue, start);
	visited[start.nodeNum] = 1;
	while (!isEmpty(queue)) {
		Node current = *dequeueNode(queue);
		Edge *nextEdge = current.edgehead;
		while (nextEdge != NULL) {
			Node nextNode = nodelist[(int)nextEdge->nodeto];
			enqueueNode(queue, nextNode);
			visited[nextNode.nodeNum] = 1;
			if (groupIndex[current.nodeNum] != -1) {
				int index = groupIndex[current.nodeNum];
				int next = 0;
				while (groups[index][next] != -1) {
					next = next + 1;
				}
				groups[index][next] = nextNode.nodeNum;
				groupIndex[nextNode.nodeNum] =index;
			} else {
				groupNum = groupNum + 1;
				groups[groupNum][0] = current.nodeNum;
				groupIndex[current.nodeNum] = groupNum;
				groups[groupNum][1] = nextNode.nodeNum;
				groupIndex[nextNode.nodeNum] = groupNum;
			}
			nextEdge = nextEdge->nextedge;
		}
	}
}