#ifndef VERTEX_H
#define VERTEX_H

#include <string>
#include <vector>

class Vertex {

public:
	std::string nom;
	std::vector<double> values;

	Vertex(std::string nom, std::vector<double> values);
	std::string toString();
};
#endif