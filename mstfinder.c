#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define maxVertices 10000
#define maxEdges 100000000

double graph[maxVertices][maxVertices];
/* Input graph must be undirected,weighted and connected*/
typedef struct Verticle {
    double xPos,yPos;
}Verticle;
Verticle V[maxVertices];

typedef struct Edge {
    int from,to;
    double weight;
}Edge;
int compare(const void * x,const void * y) {
    if((*(Edge *)x).weight < (*(Edge *)y).weight)
        return -1;
    if((*(Edge *)x).weight > (*(Edge *)y).weight)
        return 1;
    return 0;
    //return (*(Edge *)x).weight - (*(Edge *)y).weight;
}
Edge E[maxEdges];
int parent[maxVertices];
void init(int vertices) {
    int i=0;
    for(i=0;i<vertices;i++) {
        parent[i]=-1;
    }

}
int Find(int vertex) {
    if(parent[vertex]==-1) return vertex;
    return parent[vertex] = Find(parent[vertex]); /* Finding its parent as well as updating the parent 
                                                     of all vertices along this path */
}
int Union(int parent1,int parent2) {
        /* This can be implemented in many other ways. This is one of them */
        parent[parent1] = parent2;
}
void Kruskal(int vertices,int edges) {
    memset(graph,-1,sizeof(graph)); /* -1 represents the absence of edge between them */
    /* Sort the edges according to the weight */
    qsort(E,edges,sizeof(Edge),compare);
    /* Initialize parents of all vertices to be -1.*/
    init(vertices);
    int totalEdges = 0,edgePos=0,from,to;
    double weight;
    Edge now;
    while(totalEdges < vertices -1) {
        if(edgePos==edges) {
            /* Input Graph is not connected*/
            printf("Input graph is not connected");
            exit(0);
        }
        now = E[edgePos++];
        from = now.from;
        to = now.to;
        weight=now.weight;
        /* See if vertices from,to are connected. If they are connected do not add this edge. */
        int parent1 = Find(from);
        int parent2 = Find(to);
        if(parent1!=parent2) {
            graph[from][to] = weight;
            Union(parent1,parent2);
            totalEdges++;
        }
    }
}
int main(int argc, char *argv[]) {
    // Seed the random function
    srand(time(NULL));
    int vertices=0, i,j,k=0;


    for(i = 1; i < argc; i++) {
        if(strncmp(argv[i], "-test", 5) == 0) {
            printf("test mode!\n");
            exit(0);
        }
        else if(strncmp(argv[i], "-v", 2) == 0 && argc > (i+1)) {
            vertices = atoi(argv[i+1]);
        }
    }

    // If no 
    if(vertices <= 0) {
        printf("Amount of vertices to create: ");
        scanf("%d",&vertices);
        printf("\n");
    }


    for(i=0;i<vertices;i++) {
        V[i].xPos = ((double)rand()) / RAND_MAX;
        V[i].yPos = ((double)rand()) / RAND_MAX;

        for(j=0;j<i;j++) {
            E[k].from = i;
            E[k].to = j;
            E[k].weight = pow((V[i].xPos - V[j].xPos), 2.0) + pow((V[i].yPos - V[j].yPos), 2.0);
            k++;
        }
    }


    /* Finding MST */
    Kruskal(vertices,k);

    double mstLength = 0;
    /* Printing the MST */
    for(i=0;i<vertices;i++) {
        for(j=0;j<vertices;j++) {
            if(graph[i][j] > 0) {
                //printf("Vertex %d and %d, weight %f\n",i,j,graph[i][j]);
                mstLength += graph[i][j];
            }
        }
    }
    printf("The total length of the MST is %f\n\n", mstLength);
    return 0;
}