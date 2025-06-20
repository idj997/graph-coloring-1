#ifndef READMATRIX_HPP
#define READMATRIX_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

inline int** AllocateArray(const int rows, const int columns) {
  int **m = (int**) malloc(rows * sizeof(int*));
  for (int i = 0; i < rows; i++) {
    m[i] = (int*) malloc(columns * sizeof(int));
    for (int j = 0; j < columns; j++) {
      m[i][j] = 0;
    }
  }
  return m;
}

inline int** ReadFile(const char* filename, int *vertex_number) {
  std::ifstream file(filename);
  char ch;
  std::string aux;
  int** arr = NULL;
  int n = 0, i = 0, j = 0;
  while (file >> ch) {
    if (ch == 'c') {
      std::getline(file, aux);
    } else if (ch == 'p') {
      file >> aux;
      file >> n;
      arr = AllocateArray(n, n);
    } else if (ch == 'e') {
      file >> i >> j;
      arr[i-1][j-1] = 1;
      arr[j-1][i-1] = 1;
    }
  }
  *vertex_number = n;
  return arr;
}

inline int** GenerateRandomGraph(int vertices, float density) {
  int** matrix = AllocateArray(vertices, vertices);
  for (int i = 0; i < vertices; ++i) {
    for (int j = i+1; j < vertices; ++j) {
      float prob = static_cast<float>(rand()) / RAND_MAX;
      if (prob < density) {
        matrix[i][j] = matrix[j][i] = 1;
      }
    }
  }
  return matrix;
}

#endif
