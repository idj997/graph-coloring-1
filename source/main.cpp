
#include "graph.hpp"
#include "readmatrix.hpp"
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <ctime>

int main() {
    std::ofstream results_file("dsatur_results.csv");
    results_file << "Nodes,ColorsUsed,TimeNs\n";

    srand(time(0)); // Seed RNG

    for (int n = 2; n <= 5; n += 1) {
        // Create a random graph with 20% edge probability
        int** matrix = GenerateRandomGraph(n, 0.2);
        
        // Initialize graph (constructor now handles degree calculation)
        Graph g(matrix, n);

        // Run DSATUR and time it
        auto start = std::chrono::high_resolution_clock::now();
        Vertex* solution = g.dsatur();
        auto end = std::chrono::high_resolution_clock::now();
        // Calculate metrics
        int used_colors = g.count_colors(solution);
        long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        // Output results
        results_file << n << "," << used_colors << "," << duration << "\n";
        
        // Output coloring solution
        // g.exportEdges("graph_edges.csv");
        // g.exportColors("dsatur_colors.csv");
        
        // // Verify solution correctness
        // bool is_valid = true;
        // for (int i = 0; i < n && is_valid; i++) {
        //     for (int j = 0; j < n; j++) {
        //         if (matrix[i][j] != 0 && solution[i].color == solution[j].color) {
        //             is_valid = false;
        //             std::cout << "Invalid coloring: adjacent vertices " << i << " and " << j 
        //                       << " have the same color " << solution[i].color << std::endl;
        //             break;
        //         }
        //     }
        // }
        
        // if (is_valid) {
        //     std::cout << "Graph with " << n << " nodes colored successfully using " 
        //               << used_colors << " colors" << std::endl;
        // } else {
        //     std::cout << "Invalid coloring for graph with " << n << " nodes!" << std::endl;
        // }

        // Clean up memory
        for (int i = 0; i < n; ++i) free(matrix[i]);
        free(matrix);
    }

    results_file.close();
    return 0;
}