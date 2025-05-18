#include "graph.hpp"
#include "readmatrix.hpp"
#include <vector>
#include <algorithm>
#include <limits.h>
#include <fstream>
#include <iostream>
#include <cstdlib>

Graph::Graph(){
  adjacent_matrix = ReadFile("inputs/qgorder30.col", &vertices_number);
  vertices = new Vertex[vertices_number];
  for(int i=0;i<vertices_number;i++){
    vertices[i].color=-1;
    vertices[i].saturation=0;
    vertices[i].id=i;
    vertices[i].degree=0;
    vertices[i].colored=false;
  }
  for(int i=0;i<vertices_number;i++){
    for(int j=0;j<vertices_number;j++){
      if(adjacent_matrix[i][j] != 0){
        vertices[i].degree++;
      }
    }
  }
}

Graph::Graph(int** matrix, int n) {
    vertices_number = n;
    adjacent_matrix = matrix;

    vertices = new Vertex[n];
    vertex_colors = new int[n];

    for (int i = 0; i < n; i++) {
        vertices[i].id = i;
        vertices[i].saturation = 0;
        vertices[i].color = -1;
        vertices[i].colored = false;
        
        // Calculate degree for this vertex
        int degree = 0;
        for (int j = 0; j < n; j++) {
            if (adjacent_matrix[i][j] != 0) {
                degree++;
            }
        }
        vertices[i].degree = degree;
    }
}
void Graph::exportEdges(const std::string& filename) {
    std::ofstream file(filename);
    for (int i = 0; i < vertices_number; ++i) {
        for (int j = i + 1; j < vertices_number; ++j) { // to avoid duplicates
            if (adjacent_matrix[i][j] == 1) {
                file << i+1 << "," << j+1 << "\n";
            }
        }
    }
    file.close();
}

void Graph::exportColors(const std::string& filename) {
    std::ofstream file(filename);
    file << "Vertex,Color\n";
    for (int i = 0; i < vertices_number; ++i) {
        file << i + 1 << "," << vertices[i].color << "\n";
    }
    file.close();
}

bool Graph::vertex_left() {
  for(int i = 0; i < vertices_number; i++) {
    if(vertices[i].colored == false)
      return true;
  }
  return false;
}

void Graph::order_vertices(char op) {
  switch(op) {
    case 's': { // Order by saturation degree, breaking ties with degree
      int a;
      Vertex temp;
      for(int i = 0; i < vertices_number; i++) {
        a = i;
        for(int j = i + 1; j < vertices_number; j++) {
          if(vertices[j].saturation > vertices[a].saturation)
            a = j;
          else if(vertices[j].saturation == vertices[a].saturation) {
            if(vertices[j].degree > vertices[a].degree)
              a = j;
          }
        }
        temp = vertices[i];
        vertices[i] = vertices[a];
        vertices[a] = temp;
      }
      break;
    }
    case 'd': { // Order by degree only
      int a;
      Vertex temp;
      for(int i = 0; i < vertices_number; i++) {
        a = i;
        for(int j = i + 1; j < vertices_number; j++) {
          if(vertices[j].degree > vertices[a].degree)
            a = j;
        }
        temp = vertices[i];
        vertices[i] = vertices[a];
        vertices[a] = temp;
      }
      break;
    }
  }
}

// This function should be implemented to check if a color is used by any adjacent vertex
bool Graph::search_color_adjacent_dsatur(int vertex_id, int color) {
  for(int i = 0; i < vertices_number; i++) {
    if(adjacent_matrix[vertex_id][i] != 0 && vertices[i].colored && vertices[i].color == color) {
      return true; // Color is already used by an adjacent vertex
    }
  }
  return false; // Color is available
}

// Header file declaration should look like this in graph.hpp:
// void update_saturation_degrees(int colored_vertex_id);

// And in the implementation file (graph.cpp):
void Graph::update_saturation_degrees(int colored_vertex_id) {
  // For each vertex adjacent to the newly colored vertex
  for(int i = 0; i < vertices_number; i++) {
    if(adjacent_matrix[colored_vertex_id][i] != 0 && !vertices[i].colored) {
      // Count distinct colors in its neighborhood
      bool color_present[100] = {false}; // Assuming max 100 colors, adjust as needed
      int distinct_colors_count = 0;
      
      for(int j = 0; j < vertices_number; j++) {
        if(adjacent_matrix[i][j] != 0 && vertices[j].colored) {
          int adj_color = vertices[j].color;
          if(!color_present[adj_color]) {
            color_present[adj_color] = true;
            distinct_colors_count++;
          }
        }
      }
      vertices[i].saturation = distinct_colors_count;
    }
  }
}

Vertex* Graph::dsatur() {
  // Step 1: Initial ordering by degree
  order_vertices('d');
  
  // Step 2: Color the vertex with maximum degree
  vertices[0].colored = true;
  vertices[0].color = 0;
  update_saturation_degrees(vertices[0].id);
  
  // Step 3: While there are uncolored vertices
  while(vertex_left()) {
    // Step 3a: Order by saturation degree (breaking ties with degree)
    order_vertices('s');
    
    // Step 3b: Select the first uncolored vertex (highest saturation)
    int selected_vertex_idx = -1;
    for(int i = 0; i < vertices_number; i++) {
      if(!vertices[i].colored) {
        selected_vertex_idx = i;
        break;
      }
    }
    
    if(selected_vertex_idx != -1) {
      // Step 3c: Assign the smallest possible color
      int proposed_color = 0;
      while(search_color_adjacent_dsatur(vertices[selected_vertex_idx].id, proposed_color)) {
        proposed_color++;
      }
      
      vertices[selected_vertex_idx].colored = true;
      vertices[selected_vertex_idx].color = proposed_color;
      
      // Step 3d: Update saturation degrees of uncolored vertices
      update_saturation_degrees(vertices[selected_vertex_idx].id);
    }
  }
  
  return vertices; // Return the colored vertices
}
int Graph::count_colors(Vertex* vertices){
  std::vector<int> aux;
  int colors=0;
  for(int i=0;i<vertices_number;i++){
    if(!(std::find(aux.begin(), aux.end(), vertices[i].color) != aux.end())){
      aux.push_back(vertices[i].color);
      colors++;
    }
  }
  return colors;
}
int Graph::count_colors_grasp(std::vector<Vertex> vector){
  std::vector<int> aux;
  int colors=0;
  for(unsigned int i=0;i<vector.size();i++){
    if(!(std::find(aux.begin(), aux.end(), vector[i].color) != aux.end())){
      aux.push_back(vector[i].color);
      colors++;
    }
  }
  return colors;
}
void Graph::print_graph_coloring(){
  for(int i=0;i<vertices_number;i++)
  {
      std::cout<<"V"<<vertices[i].id+1<<" C"<<vertices[i].color<<std::endl;
  }
}
bool Graph::search_color_adjacent(const int id,const int color){
  for(int j=0;j<vertices_number;j++){
    if(adjacent_matrix[id][j] != 0)
    {
      if(vertices[j].color == color)
        return true;
    }
  }
  return false;
}
// bool Graph::search_color_adjacent_dsatur(const int id,const int color){
//   for(int j=0;j<vertices_number;j++){
//     if(adjacent_matrix[id][j] != 0)
//     {
//       for(int i=0;i<vertices_number;i++){
//         if(vertices[i].id == j && vertices[i].color == color){
//             return true;
//         }
//       }
//     }
//   }
//   return false;
// }
Vertex* Graph::heuristic_constructor(){
    std::cout<<"--------------CONSTRUCTOR-------------"<<std::endl;
    for(int i=0;i<vertices_number;i++){
      int proposed_color=0;
      while(search_color_adjacent(i,proposed_color)){
        proposed_color++;
      }
      vertices[i].color=proposed_color;
    }
    int color_count=count_colors(vertices);
    //print_graph_coloring();
    std::cout<<color_count<<std::endl;
    return vertices;
}
bool Graph::check_vertices_color(const int color_in,const int color_out,int* vertex_colors){
  for(int i=0;i<vertices_number;i++){
    if(vertex_colors[i] == color_in){
      if(search_color_adjacent(i,color_out))
        return false;
    }
  }
  return true;
}
 
