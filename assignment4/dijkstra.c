/*
 * In this file, you'll implement Dijkstra's algorithm to find the least
 * expensive paths in the graph defined in `airports.dat`.  Don't forget to
 * include your name and @oregonstate.edu email address below.
 *
 * Name: Lianghui Wang
 * Email: wangl9@oregonstate.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define DATA_FILE "airports.dat"

int minDistance(int dist[], int sptSet[], int n_nodes) {
    int min = INT_MAX, min_index = -1;
    for (int v = 0; v < n_nodes; v++) {
        if (!sptSet[v] && dist[v] <= min) {
            min = dist[v], min_index = v;
        }
    }
    return min_index;
}

void printSolution(int dist[], int n_nodes) {
    printf("Vertex \t Distance from Source\n");
    for (int i = 0; i < n_nodes; i++) {
        if (dist[i] == INT_MAX) {
            printf("%d \t\t %s\n", i, "Unreachable");
        } else {
            printf("%d \t\t %d\n", i, dist[i]);
        }
    }
}

void dijkstra(int **graph, int src, int n_nodes) {
    int dist[n_nodes]; 
    int sptSet[n_nodes];

    for (int i = 0; i < n_nodes; i++) {
        dist[i] = INT_MAX, sptSet[i] = 0;
    }

    dist[src] = 0;

    for (int count = 0; count < n_nodes - 1; count++) {
        int u = minDistance(dist, sptSet, n_nodes);

        if (u == -1) break; // If no vertex is left to process, break the loop

        sptSet[u] = 1;

        for (int v = 0; v < n_nodes; v++) {
            if (!sptSet[v] && graph[u][v] != INT_MAX && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    printSolution(dist, n_nodes);
}

int main(int argc, char const *argv[]) {
	/*
	 * Open file and read the first two int: num of nodes, num of edges
	 */

	int n_nodes, n_edges;
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    fscanf(file, "%d %d", &n_nodes, &n_edges);

    int **graph = (int **)malloc(n_nodes * sizeof(int *));
    for (int i = 0; i < n_nodes; i++) {
        graph[i] = (int *)malloc(n_nodes * sizeof(int));
        for (int j = 0; j < n_nodes; j++) {
            graph[i][j] = INT_MAX;
        }
    }

    int src, dest, cost;
    while (fscanf(file, "%d %d %d", &src, &dest, &cost) == 3) {
        graph[src][dest] = cost;
    }
    fclose(file);

    int start_node;
    printf("Enter the source node (0 to %d): ", n_nodes - 1);
    if (scanf("%d", &start_node) != 1) {
        printf("Invalid input.\n");
        return -1;
    }

    if (start_node < 0 || start_node >= n_nodes) {
        printf("Source node must be between 0 and %d.\n", n_nodes - 1);
        return -1;
    }

    dijkstra(graph, start_node, n_nodes);

    for (int i = 0; i < n_nodes; i++) {
        free(graph[i]);
    }
    free(graph);

    return 0;
}
