/*
To set the correct locations:
$ LD_LIBRARY_PATH=/usr/local/lib
$ export LD_LIBRARY_PATH

Example run
$ gcc mstfinder.c -lgsl -lgslcblas -lm
$./a.out -fast -v 75000
Running in fast mode; results can not be guaranteed to be correct.
Reserved space for 400000000 edges.
There are 383592578 edges used for this graph, of 5624925000 possible edges (6.82%)
 
start qsort
finished qsort
The total length of the MST is 0.508182
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gsl/gsl_rng.h>
#include <stdbool.h>
#define threshold 0.05
#define maxVertices 80000
/* experimentally, maxEdges should be at least: (threshold * maxVertices^2 * 4)
   actually more like like PI, but to be on the safe side... */
#define maxEdges (maxVertices * 1250 * 4)

// To create the random
const gsl_rng_type * T;
gsl_rng * r;

/* Input graph must be undirected,weighted and connected*/
typedef struct Verticle {
    double xPos,yPos;
}Verticle;
Verticle V[maxVertices];

bool silent = false;
double chance = 0.5;

typedef struct Edge {
    int from,to;
    double weight;
}Edge;
int compare(const void * x,const void * y) {
    if((*(Edge *)x).weight < (*(Edge *)y).weight) return -1;
    if((*(Edge *)x).weight > (*(Edge *)y).weight) return 1;
    return 0;
}
Edge E[maxEdges]; // Educated guess of the max amount of edges
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

double Kruskal(int vertices,int edges) {
    /* Sort the edges according to the weight */
    if(!silent) { printf("start qsort\n"); }
    qsort(E,edges,sizeof(Edge),compare);
    if(!silent) { printf("finished qsort\n"); }

    /* Initialize parents of all vertices to be -1.*/
    init(vertices);
    int totalEdges = 0,edgePos=0,from,to;
    double weight, mstLength = 0;
    double maxWeight = 0;
    Edge now;
    int iteration = 0;
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
            if(maxWeight < weight) {
                maxWeight = weight;
            }
            mstLength += weight;
            Union(parent1,parent2);
            totalEdges++;
        }
        iteration++;
    }

    if(!silent) { printf("The longest edge is %f\n", maxWeight); }
    else        { printf("max(|e|) = %f   ", maxWeight);         }

    return mstLength;
}

double genMST(int vertices, bool full_run) {
    int i,j,k=0;
    
    // If no argument was supplied
    if(vertices <= 0) {
        printf("Amount of vertices to create: ");
        scanf("%d",&vertices);
        printf("\n");
    }

    for(i=0;i<vertices;i++) {
        V[i].xPos = ((double) gsl_rng_uniform(r));
        V[i].yPos = ((double) gsl_rng_uniform(r));

        for(j=0;j<i;j++) {
            if(((double)rand()) / RAND_MAX < chance) {
                double weight = pow((V[i].xPos - V[j].xPos), 2.0) + pow((V[i].yPos - V[j].yPos), 2.0);
                if(weight < threshold) {
                    E[k].from = i;
                    E[k].to = j;
                    E[k].weight = weight;
                    k++;
                }
            }
        }
    }

    if(!silent) { printf("There are %d edges used for this graph, of %llu possible edges (%.3g%%)\n\n", k*2, ((long) vertices*(vertices-1)), 100*((double)k*2/vertices/(vertices-1)) ); }

    /* Finding MST */
    return Kruskal(vertices,k);
}

int main(int argc, char *argv[]) {
    // Set up the random function
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    gsl_rng_set(r, time(NULL));

    int i, vertices = 0;
    bool full_run = true; // Whether or not to skip long edges

    for(i = 1; i < argc; i++) {
        if(strncmp(argv[i], "-test", 5) == 0) {
            printf("test mode!\n");
            exit(0);
        }
        else if(strncmp(argv[i], "-fast", 5)   == 0) { full_run = false; }
        else if(strncmp(argv[i], "-silent", 7) == 0) { silent = true;   }
        else if(strncmp(argv[i], "-v", 2) == 0 && argc > (i+1)) { vertices = atoi(argv[i+1]); }
        else if(strncmp(argv[i], "-p", 2) == 0 && argc > (i+1)) { chance   = atof(argv[i+1]); }
    }

    if((!silent) && full_run) { printf("Running in fast mode; results can not be guaranteed to be correct.\n"); }
    if(!silent)               { printf("Reserved space for %d edges.\n", maxEdges); }

    double mstLength = genMST(vertices, full_run);
    
    if(!silent) { printf("The total length of the MST is %f\n\n", mstLength); }
    else        { printf("#V=%i    L=%f\n", vertices, mstLength); }
    
    return 0;
}