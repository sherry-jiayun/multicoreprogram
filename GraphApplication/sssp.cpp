#include <iostream>
#include <cstdlib>
#include <limits>

using namespace std;

const int gsize = 10;
const int scource = 0;

int shortestvertex(int distance[gsize], bool connection[gsize]){
	int index = 0;
	int min = std::numeric_limits<int>::max();
	for(int i = 0; i < gsize; i++){
		if(connection[i] == false && distance[i] < min){
			min = distance[i];
			index = i;
		}
	}
	return index;
}



void dijkstra(int edgeweight[gsize][gsize], int scource){
    //an array to set original distance from single scource to every vertics
    int distance[gsize];
    distance[scource] = 0;
    for(int i = 1; i < gsize; i++){
    	distance[i] = std::numeric_limits<int>::max();
    }

    bool connection[gsize];
    for(int i = 0; i < gsize; i ++){
    	connection[i] = false;
    }

    for(int i = 0; i < gsize; i++){
    	int nextV = shortestvertex(distance, connection);
    	connection[nextV] = true;

    	for(int j = 0; j < gsize; j++){
    		if(connection[j] == false && edgeweight[i][j] > 0 && 
    			distance[i] + edgeweight[i][j] < distance[j]){
    			distance[j] = distance[i] + edgeweight[i][j];
    		}
    	}
    }


    for(int i = 0; i < gsize; i++){
    	cout << distance[i] << endl;
    }
}

int main()
{
    int edgeweight[gsize][gsize];

    //read input
    for(int i = 0; i < gsize; i ++){
    	for(int j = i+1; j < gsize; j++){
    		int r = rand() % 10;
    		edgeweight[i][j] = r;
    		edgeweight[j][i] = r;
    	}
    	edgeweight[i][i] = 0;

    }
    // print graph
    for(int i = 0; i < gsize; i ++){
    	for(int j = 0; j < gsize; j++){
    		cout << edgeweight[i][j];
    	}
    	cout << "" << endl;
    }


    dijkstra(edgeweight, scource);
    return 0;

}



















