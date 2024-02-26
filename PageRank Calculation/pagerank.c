#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define MAX_NODES 40000
#define DAMPING_FACTOR 0.85
#define MAX_ITERATIONS 50

typedef struct {
    double pagerank;
    double new_pagerank;
    int num_outgoing_edges;
    int* outgoing_edges;
    pthread_mutex_t lock;
} Node;

typedef struct {
   Node* nodes;
   int start,end;
}FunArg;

pthread_barrier_t barrier;

void* pagerankcalc(void* arg){
    FunArg* argument = (FunArg*)arg;
    Node* nodes = argument->nodes;
    int start = argument->start;
    int end = argument->end;
    double tmp;
    int outgoing;
    
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        for (int i = start; i < end; i++) {
            if(nodes[i].num_outgoing_edges==0){
                continue;
            }
            tmp = nodes[i].pagerank / nodes[i].num_outgoing_edges;
            for (int j = 0; j < nodes[i].num_outgoing_edges; j++) { 
                outgoing = nodes[i].outgoing_edges[j]; 
                pthread_mutex_lock (&nodes[outgoing].lock); 
                nodes[outgoing].new_pagerank += tmp;
                pthread_mutex_unlock (&nodes[outgoing].lock); 
            }
        }
        pthread_barrier_wait(&barrier);
        for (int i = start; i < end; i++) {
            nodes[i].pagerank = DAMPING_FACTOR * nodes[i].new_pagerank + (1 - DAMPING_FACTOR);
            nodes[i].new_pagerank = 0;
        }
        pthread_barrier_wait(&barrier);
    }
}


int main(int argc, char* argv[]) {
    if(argc != 3){
        printf("Exceeded maximum number of arguments\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    int NUM_THREADS = atoi(argv[2]);

    if (file == NULL) {
        fprintf(stderr, "Could not open file\n");
        return 1;
    }
    fprintf(stderr, "File opened successfully.\n");

    int num_nodes = 0;
    Node nodes[MAX_NODES];
    long src, dst;
    pthread_t threads[NUM_THREADS];
    FunArg args[NUM_THREADS];
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    for(int i = 0; i<MAX_NODES; i++){
        nodes[i].outgoing_edges = malloc(sizeof(int)*MAX_NODES);
    }
    while (fscanf(file, "%ld %ld", &src, &dst) == 2) {
        if (num_nodes >= MAX_NODES) {
            fprintf(stderr, "Exceeded maximum number of nodes\n");
            return 1;
        }
        nodes[src].outgoing_edges[nodes[src].num_outgoing_edges++] = dst;
        num_nodes = num_nodes < src ? src : num_nodes;
        num_nodes = num_nodes < dst ? dst : num_nodes;
    }
    num_nodes++;
    fclose(file);

    printf("Graph loaded successfully. Number of nodes: %d\n", num_nodes);


    for (int i = 0; i < num_nodes; i++) {
        nodes[i].pagerank = 1.0;
    }

    printf("PageRank initialized for all nodes\n");

    for(int i = 0; i<NUM_THREADS; i++){
        args[i].nodes = nodes;
        args[i].start = (num_nodes/NUM_THREADS) * i;
        args[i].end = (num_nodes/NUM_THREADS) * (i+1);
        args[i].end = (i != NUM_THREADS - 1 ? (num_nodes / NUM_THREADS) * (i + 1) : num_nodes);
        pthread_create(&threads[i], NULL, pagerankcalc, &args[i]);
    }
    for(int i = 0; i<NUM_THREADS; i++){
        pthread_join(threads[i],NULL);
    }


    FILE *output_file = fopen("pagerank.csv", "w");
    if (output_file == NULL) {
        fprintf(stderr, "Could not open output file\n");
        return 1;
    }
    fprintf(output_file, "node,pagerank\n");
    for (int i = 0; i < num_nodes; i++) {
        fprintf(output_file, "%d,%f\n", i, nodes[i].pagerank);
    }
    fclose(output_file);

    printf("PageRank calculation completed. Results written to pagerank.csv\n");

    for(int i = 0; i<MAX_NODES; i++){
        free(nodes[i].outgoing_edges);
    }
    
    pthread_barrier_destroy(&barrier);
    return 0;
}
