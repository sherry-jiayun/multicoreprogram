#include "loadgraph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <omp.h>

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

//Global variable
int numOfNode;
int numOfThreads;

void connectedComponents_m(int **graphmatric);
void connectedComponents_a(Node *nodelist);
void connectedComponents_a_parallel(Node *nodelist);
void hook(int i, int j, int parent[numOfNode]);
void star(int vertex, int star[numOfNode], int parent[numOfNode]);
void connectedComponents_m_parallel(int **graphmatric);

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
    //printf("%d enqueued to queue\n", item);
}

//Function to add a node to the queue
void enqueueNode(struct Queue* queue, Node item) {
	if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->nodes[queue->rear] = item;
    queue->size = queue->size + 1;
    //printf("%d enqueued to queue\n", item.nodeNum);
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

	strcpy(mode_matrix, "m");
	strcpy(mode_adjlist, "l");
	if(argc == 5){
		numOfNode = getInt(argv[1]);
		numOfThreads = getInt(argv[4]);
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
			//for (int i = 0; i < numOfNode; i++) {
				//printf("Node No.%d\t",i);
				//for (int j = 0; j < numOfNode; j++) {
					//printf("%d  ",graphmatric[i][j]);
				//}
				//printf("\n");
			//}
			//connectedComponents_m(graphmatric);
			connectedComponents_m_parallel(graphmatric);
			free(graphmatric);
		} else {
			result = strcmp(mode_adjlist, mode);
			if (result == 0) {
				Node nodelist[numOfNode]; // number of node array
				createAdjList(nameOfFile, numOfNode, (Node *) &nodelist);
				// loop to go through all list
				int i; 
				for (i = 0; i < numOfNode; i++) {
					Node ntmp = (Node)nodelist[i];
					Edge *etmp = (Edge *)ntmp.edgehead;
					printf("Node num: %d\n", ntmp.nodeNum);
					while (etmp != NULL) {
						printf("[from: %d to: %d weight: %d \t", etmp->nodefrom,etmp->nodeto,etmp->weight );
						etmp = etmp->nextedge;
					}
					printf("\n");
				}
			//connectedComponents_a(nodelist);	
			//connectedComponents_a_parallel(nodelist);
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

void connectedComponents_m(int **graphmatric) {
	struct Queue* queue = createQueue(numOfNode);
	int visited[numOfNode];
	int i;
	int node;

	//initialization
	for (i = 0; i < numOfNode; i++) {
		visited[i] = 0;
	}

	for (node = 0; node < numOfNode; node++) {
		if (!visited[node]) {
			//BFS
			enqueue(queue, node);
			printf("%d ", node);
			visited[node] = 1;
			while (!isEmpty(queue)) {
				int current = dequeue(queue);
				for (i = 0; i < numOfNode; i++) {
					if (graphmatric[current][i] == 0 || visited[i] == 1) {
						continue;
					}
					visited[i] = 1;
					enqueue(queue, i);
					printf("%d ", i);
				}
			}
			printf("\n");
		}
	}
}

void connectedComponents_m_parallel(int **graphmatric) {
	int parent[numOfNode];
	int isStar[numOfNode];
	int i;
	int j;
	int noChange = 0;

	omp_set_num_threads(numOfThreads);

	//initialization
	#pragma omp parallel num_threads (numOfThreads)
	for (i = 0; i < numOfNode; i++) {
		parent[i] = i; 
	}

	for (i = 0; i < numOfNode; i++) {
		#pragma omp parallel num_threads (numOfThreads)
		for (j = 0; j < numOfNode; j++) {
			if(graphmatric[i][j] != 0) {
				int from = i;
                        	int to = j;
				if (from > to) {
					hook(from, to, parent);
				}
				if (parent[from] == from) {
					hook(from, to, parent);
				}
			}
		}
	}
	
	while (!noChange) {
		#pragma omp parallel num_threads (numOfThreads)
		for (i = 0; i < numOfNode; i++) {
			star(i, isStar, parent);
		}
		for (i = 0; i < numOfNode; i++) {
			#pragma omp parallel num_threads (numOfThreads)
			for (j = 0; j < numOfNode; j++) {	
				if (graphmatric[i][j] != 0) {
					int from = i;
					int to = j;
					if (isStar[from] && parent[from] > parent[to]) {
						hook(from, to, parent);
					}
				}
			}
		}
		#pragma omp parallel num_threads (numOfThreads)
		for (i = 0; i < numOfNode; i++) {
			star(i, isStar, parent);
		}
		for (i = 0; i < numOfNode; i++) {
			#pragma omp parallel num_threads (numOfThreads)
			for (j = 0; j < numOfNode; j++) {	
				if (graphmatric[i][j] != 0) {
					int from = i;
					int to = j;
					if (isStar[from] && parent[from] != parent[to]) {
						hook(from, to, parent);
					}
				}
			}
		}
		#pragma omp parallel num_threads (numOfThreads)
		for (i = 0; i < numOfNode; i++) {
			noChange = 1;
			if (parent[i] != parent[parent[i]]) {
				parent[i] = parent[parent[i]];
				noChange = 0;
			}
		}
	}
	
	for (i = 0; i < numOfNode; i++) {
		printf("Node: %d, Parent: %d\n", i, parent[i]);
	}
}

void connectedComponents_a_parallel(Node *nodelist) {
	int parent[numOfNode];
	int isStar[numOfNode];
	int i;
	int noChange = 0;

	omp_set_num_threads(numOfThreads);

	//initialization
	#pragma omp parallel num_threads (numOfThreads)
	for (i = 0; i < numOfNode; i++) {
		parent[i] = i; 
	}

	#pragma omp parallel num_threads (numOfThreads)
	for (i = 0; i < numOfNode; i++) {
		Node curr = nodelist[i];
		Edge *nextEdge = curr.edgehead;
		while (nextEdge != NULL) {
			int from = nextEdge->nodefrom;
			int to = nextEdge->nodeto;
			if (from > to) {
				hook(from, to, parent);
			}
			if (parent[from] == from) {
				hook(from, to, parent);
			}
			nextEdge = nextEdge->nextedge;
		}
	}
	
	while (!noChange) {
		#pragma omp parallel num_threads (numOfThreads)
		for (i = 0; i < numOfNode; i++) {
			star(i, isStar, parent);
		}
		#pragma omp parallel num_threads (numOfThreads)
		for (i = 0; i < numOfNode; i++) {
			Node curr = nodelist[i];
			Edge *nextEdge = curr.edgehead;
			while (nextEdge != NULL) {
				int from = nextEdge->nodefrom;
				int to = nextEdge->nodeto;
				if (isStar[from] && parent[from] > parent[to]) {
					hook(from, to, parent);
				}
				nextEdge = nextEdge->nextedge;
			}
		}
		#pragma omp parallel num_threads (numOfThreads)
		for (i = 0; i < numOfNode; i++) {
			star(i, isStar, parent);
		}
		#pragma omp parallel num_threads (numOfThreads)
		for (i = 0; i < numOfNode; i++) {
			Node curr = nodelist[i];
			Edge *nextEdge = curr.edgehead;
			while (nextEdge != NULL) {
				int from = nextEdge->nodefrom;
				int to = nextEdge->nodeto;
				if (isStar[from] && parent[from] != parent[to]) {
					hook(from, to, parent);
				}
				nextEdge = nextEdge->nextedge;
			}
		}
		#pragma omp parallel num_threads (numOfThreads)
		for (i = 0; i < numOfNode; i++) {
			noChange = 1;
			if (parent[i] != parent[parent[i]]) {
				parent[i] = parent[parent[i]];
				noChange = 0;
			}
		}
	}
	
	for (i = 0; i < numOfNode; i++) {
		printf("Node: %d, Parent: %d\n", i, parent[i]);
	}
}

void hook(int from, int to, int parent[numOfNode]) {
	parent[parent[from]] = parent[to];
}

void star(int vertex, int star[numOfNode], int parent[numOfNode]) {
	star[vertex] = 1;
	if (parent[vertex] != parent[parent[vertex]]) {
		star[vertex] = 0;
		star[parent[parent[vertex]]] = 0;
	}
	if (star[parent[vertex]] == 0) {
		star[vertex] = 0;
	}
}

void connectedComponents_a(Node *nodelist) {
	struct Queue* queue = createQueue(numOfNode);
	int visited[numOfNode];
	int i;

	//initialization
	for (i = 0; i < numOfNode; i++) {
		visited[i] = 0;
	}

	Node start = (Node) nodelist[0];

	for (i = 0 ; i < numOfNode; i++) {
		//BFS
		start = (Node) nodelist[i];
		if (!visited[start.nodeNum]) {
			enqueueNode(queue, start);
			printf("%d ", start.nodeNum);
			visited[start.nodeNum] = 1;
			while (!isEmpty(queue)) {
				Node current = *dequeueNode(queue);
				Edge *nextEdge = current.edgehead;
				while (nextEdge != NULL && !visited[nextEdge->nodeto]) {
					Node nextNode = nodelist[(int)nextEdge->nodeto];
					if (!visited[nextNode.nodeNum]) {
						enqueueNode(queue, nextNode);
						printf("%d ", nextNode.nodeNum);
						visited[nextNode.nodeNum] = 1;
						nextEdge = nextEdge->nextedge;
					}
				}
			}
		}
		printf("\n");
	}
}
