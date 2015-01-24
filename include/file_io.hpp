#ifndef _FILE_IO_HPP_
#define _FILE_IO_HPP_

#include "common.hpp"

#include <string>
#include <vector>

// Read the list of triples from the file specified by file_name.
std::vector<Tuple> ReadTriples(std::string file_name);

// Read the list of triples from the file specified by file_name.
std::vector<Tuple> ReadTriplesAndDuplicate(std::string file_name);


// Read the list of triples from the file specified by file_name.
template <typename T>
void WriteVector(std::vector<T>& vec, std::string file_name);


// Read the list of triples from the file specified by file_name.
template <typename T>
std::vector<T> ReadVector(std::string file_name);

#endif  // _FILE_IO_HPP_
