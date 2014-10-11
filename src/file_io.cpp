#include "file_io.hpp"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<Tuple> ReadTriples(std::string file_name) {
  std::cout << file_name << std::endl;
  FILE *file = fopen(file_name.c_str(), "r");
  assert(file != NULL);
  std::vector<Tuple> triples;

  // All triples are just space-separated, one per line.
  int u, v, w;
  while (fscanf(file, "%d %d %d", &u, &v, &w) == 3) {
    assert(u >= 0 && v >= 0 && w >= 0);
    triples.push_back(Tuple(u, v, w));
  }

  std::cout << "Number of triples read: " << triples.size() << std::endl;

  return triples;
}


template <typename T>
void WriteVector(std::vector<T>& vec, std::string file_name) {
  std::ofstream outfile(file_name.c_str(), std::ios::out | std::ios::trunc);
  for (T entry : vec) {
    outfile << entry << std::endl;
  } 
}


template <typename T>
std::vector<T> ReadVector(std::string file_name) {
    std::ifstream infile(file_name.c_str());
    std::string line;
    std::vector<T> vec;

    while (std::getline(infile, line)) {
	std::istringstream iss(line);
	T val;
	while (iss >> val) {
	    vec.push_back(val);
	}
    }

    return vec;
}

template void WriteVector(std::vector<int>& vec, std::string file_name);
template void WriteVector(std::vector<double>& vec, std::string file_name);
template void WriteVector(std::vector<float>& vec, std::string file_name);

template std::vector<int> ReadVector(std::string file_name);
template std::vector<double> ReadVector(std::string file_name);
template std::vector<float> ReadVector(std::string file_name);
